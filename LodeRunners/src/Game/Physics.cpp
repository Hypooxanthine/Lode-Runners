#include "Physics.h"

#include "Components/ColliderComponent.h"

#include "Entities/Pawns/Pawn.h"

Physics* Physics::m_Instance = nullptr;

Physics::Physics()
{
	ASSERT(m_Instance == nullptr, "Physics singleton already exists.");

	m_Instance = this;
}

void Physics::update()
{
	
}

void Physics::registerCollider(ColliderComponent* collider)
{
	m_Colliders.push_back(collider);

	// We cache dynamic and static colliders to perform faster collision resolving.
	if (collider->getCollisionType() == CollisionType::Dynamic)
		m_DynamicColliders.push_back(collider);
	else if (collider->getCollisionType() == CollisionType::Static)
		m_StaticColliders.push_back(collider);
}

void Physics::unregisterCollider(ColliderComponent* collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);

	if (it != m_Colliders.end())
		m_Colliders.erase(it);

	if (collider->getCollisionType() == CollisionType::Dynamic)
	{
		it = std::find(m_DynamicColliders.begin(), m_DynamicColliders.end(), collider);

		if (it != m_DynamicColliders.end())
			m_DynamicColliders.erase(it);
	}
	else if(collider->getCollisionType() == CollisionType::Static)
	{
		it = std::find(m_StaticColliders.begin(), m_StaticColliders.end(), collider);

		if (it != m_StaticColliders.end())
			m_StaticColliders.erase(it);
	}
}

void Physics::updateColliderType(ColliderComponent* collider)
{
	std::vector<ColliderComponent*>::iterator it;

	// Checking if collider was stored in static or dynamic colliders (and removing it).
	it = std::find(m_DynamicColliders.begin(), m_DynamicColliders.end(), collider);

	if (it != m_DynamicColliders.end())
		m_DynamicColliders.erase(it);
	else
	{
		it = std::find(m_StaticColliders.begin(), m_StaticColliders.end(), collider);

		if (it != m_StaticColliders.end())
			m_StaticColliders.erase(it);
	}

	if (collider->getCollisionType() == CollisionType::Dynamic)
		m_DynamicColliders.push_back(collider);
	else if (collider->getCollisionType() == CollisionType::Static)
		m_StaticColliders.push_back(collider);
}

void Physics::fixEntityPosition(Entity* entity) const
{
	auto collider = entity->getComponent<ColliderComponent>();

	if (!collider || collider.value()->getCollisionType() != CollisionType::Dynamic) return;

	sf::Vector2f contactPoint, contactNormal;
	float t = 0.f;
	std::vector<std::pair<size_t, float>> collisions;

	for (size_t j = 0; j < m_Colliders.size(); j++)
	{
		if (m_Colliders[j]->getCollisionType() != CollisionType::Static) continue;

		if (dynamicWithStatic(collider.value(), m_Colliders[j], collider.value()->getLastPosition(), contactPoint, contactNormal, t))
		{
			collisions.push_back({ j, t });
		}
	}

	std::sort
	(
		collisions.begin(), collisions.end(),
		[](const std::pair<size_t, float>& a, const std::pair<size_t, float>& b)
		{
			return a.second < b.second;
		}
	);

	for (auto& c : collisions)
		resolveDynamicWithStatic(collider.value(), m_Colliders[c.first]);

	collider.value()->m_LastPosition = collider.value()->getWorldPosition();

}

bool Physics::pointInCollider(const sf::Vector2f& point, const ColliderComponent* collider) const
{
	auto rect = collider->getHitRect();
	return point.x >= rect.left && point.x <= rect.left + rect.width
		&& point.y >= rect.top && point.y <= rect.top + rect.height;
}

bool Physics::colliderWithCollider(const ColliderComponent* a, const ColliderComponent* b) const
{
	auto r1 = a->getHitRect();
	auto r2 = b->getHitRect();

	return r1.left < r2.left + r2.width  && r1.left + r1.width  > r2.left
		&& r1.top  < r2.top  + r2.height && r1.top  + r1.height > r2.top;
}

bool Physics::rayWithHitbox(const Ray& ray, const sf::FloatRect& hitbox, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear) const
{
	contactPoint = { 0.f, 0.f };
	contactNormal = { 0.f, 0.f };

	// Multiplication is faster than division so we cache it.
	sf::Vector2f invDir = { 1.f / ray.direction.x, 1.f / ray.direction.y };

	sf::Vector2f tNear =
	{
		(hitbox.left - ray.origin.x) * invDir.x,
		(hitbox.top - ray.origin.y) * invDir.y
	};
	sf::Vector2f tFar =
	{
		(hitbox.left + hitbox.width - ray.origin.x) * invDir.x,
		(hitbox.top + hitbox.height - ray.origin.y) * invDir.y
	};

	if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
	if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;

	if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
	if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

	if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

	tHitNear = std::max(tNear.x, tNear.y);
	float tHitFar = std::min(tFar.x, tFar.y);

	if (tHitFar < 0.f) return false;

	// C(t) = O + t * D, Contact = C(t) with t = near time collision.
	contactPoint = ray.origin + tHitNear * ray.direction;

	if (tNear.x > tNear.y)
	{
		if (invDir.x < 0.f)
			contactNormal = { 1.f, 0.f };
		else
			contactNormal = { -1.f, 0.f };
	}
	else if (tNear.x < tNear.y)
	{
		if (invDir.y < 0.f)
			contactNormal = { 0.f, 1.f };
		else
			contactNormal = { 0.f, -1.f };
	}

	return true;
}

bool Physics::dynamicWithStatic(const ColliderComponent* dynColl, const ColliderComponent* statColl, const sf::Vector2f& previousPos, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime) const
{
	auto dynRect = dynColl->getHitRect();

	// If there wasn't any movement
	if (sf::Vector2f(dynRect.left, dynRect.top) == previousPos)
	{
		return false;
	}

	auto statRect = statColl->getHitRect();

	// This is static hitbox expendend by the half size of the dynamic hitbox. It'll allow the dynamic hitbox to clip to the static one.
	sf::FloatRect expendedStatRect
	(
		statColl->getWorldPosition() - dynColl->getHitbox() / 2.f,
		statColl->getHitbox() + dynColl->getHitbox()
	);

	// The ray starts from the center of the previous dynamic hitbox and ends on the center of the current dynamic hitbox.
	// Long story short, it is the displacement of the hitbox.
	Ray ray =
	{
		// Center of the hitbox
		previousPos + dynColl->getHitbox() / 2.f,
		dynColl->getWorldPosition() - previousPos
	};

	if (rayWithHitbox(ray, expendedStatRect, contactPoint, contactNormal, contactTime))
		return contactTime >= 0.f && contactTime <= 1.f;
	else
		return false;
}

void Physics::resolveDynamicWithStatic(ColliderComponent* dynColl, ColliderComponent* statColl) const
{
	// Here will be stored contact point and normal between dynamic Collider and static Collider.
	sf::Vector2f contactPoint, contactNormal;

	// Here will be stored the contact "time" between dynamic Collider and static Collider. This "time" is the required percentage of the dynamic Collider displacement that leads to a clipping to the static Collider.
	float contactTime = 0.f;

	// If there is no collision, previous values are meaningless.

	if(dynColl->blocks(statColl))
	{
		if (dynamicWithStatic(dynColl, statColl, dynColl->getLastPosition(), contactPoint, contactNormal, contactTime))
			dynColl->getParent()->m_WorldPosition = (contactPoint - dynColl->getRelativePosition() - dynColl->getHitbox() / 2.f) * SPACE_UNIT;
	}
	else if (dynColl->overlaps(statColl) || statColl->overlaps(dynColl))
	{
		if (dynamicWithStatic(dynColl, statColl, dynColl->getLastPosition(), contactPoint, contactNormal, contactTime))
		{
			if(dynColl->overlaps(statColl))
				dynColl->notifyOverlaps(statColl);

			if (statColl->overlaps(dynColl))
				statColl->notifyOverlaps(dynColl);
		}
	}
	
}
