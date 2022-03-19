#include "Physics.h"

#include "Components/ColliderComponent.h"

Physics* Physics::m_Instance = nullptr;

Physics::Physics()
{
	ASSERT(m_Instance == nullptr, "Physics singleton already exists.");

	m_Instance = this;
}

void Physics::update()
{
	for (size_t i = 0; i < m_Colliders.size(); i++)
	{
		for (size_t j = i + 1; i < m_Colliders.size(); j++)
		{
			m_Colliders[i]->resolveCollisionWith(m_Colliders[j]);
		}
	}
}

void Physics::registerCollider(ColliderComponent* collider)
{
	m_Colliders.push_back(collider);
}

bool Physics::unregisterCollider(ColliderComponent* collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);

	if (it != m_Colliders.end())
	{
		m_Colliders.erase(it);
		return true;
	}

	return false;
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
	if (tNear.y < tFar.y) std::swap(tNear.y, tFar.y);

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
		if (invDir.x < 0.f)
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
	if (sf::Vector2f(dynRect.left, dynRect.top) == previousPos) return false;

	auto statRect = dynColl->getHitRect();

	// This is static hitbox expendend by the half size of the dynamic hitbox. It'll allow the dynamic hitbox to clip to the static one.
	sf::FloatRect expendedStatRect
	(
		statRect.left - dynRect.width / 2.f,
		statRect.top - dynRect.top / 2.f,
		statRect.width + dynRect.width,
		statRect.height + dynRect.height
	);

	// The ray starts from the center of the previous dynamic hitbox and ends on the center of the current dynamic hitbox.
	// Long story short, it is the displacement of the hitbox.
	Ray ray =
	{
		// Center of the hitbox
		previousPos + sf::Vector2f(dynRect.width, dynRect.height) / 2.f,
		sf::Vector2f(dynRect.left, dynRect.top) - previousPos
	};

	if (rayWithHitbox(ray, expendedStatRect, contactPoint, contactNormal, contactTime))
		return contactTime >= 0.f && contactTime <= 1.f;
	else
		return false;
}

bool Physics::resolveDynamicWithStatic(ColliderComponent* dynColl, ColliderComponent* statColl, const sf::Vector2f& previousPos) const
{
	// Here will be stored contact point and normal between dynamic Collider and static Collider.
	sf::Vector2f contactPoint, contactNormal;

	// Here will be stored the contact "time" between dynamic Collider and static Collider. This "time" is the required percentage of the dynamic Collider displacement that leads to a clipping to the static Collider.
	float contactTime = 0.f;

	// If there is no collision, previous values are meaningless.

	if (dynamicWithStatic(dynColl, statColl, previousPos, contactPoint, contactNormal, contactTime))
	{
		auto& hitbox = dynColl->getHitbox();

		// Collider has to be here.
		sf::Vector2f fixedPosition = contactPoint + sf::Vector2f(contactNormal.x * hitbox.x, contactNormal.y * hitbox.y);
		sf::Vector2f move = fixedPosition - dynColl->getWorldPosition();

		dynColl->getParent()->move(move);
	}
	else
		return false;
}

bool Physics::pointInRect(const sf::Vector2f& point, const sf::FloatRect& rect)
{
	return point.x >= rect.left && point.x <= rect.left + rect.width
		&& point.y >= rect.top && point.y <= rect.top + rect.height;
}

bool Physics::rectCrossesRect(const sf::FloatRect& a, const sf::FloatRect& b)
{
	return a.left < b.left + b.width && a.left + a.width > b.left
		&& a.top < b.top + b.height && a.top + a.height > b.top;
}

bool Physics::rayThroughRect(const Physics::Ray& ray, const sf::FloatRect& rect, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear)
{
	contactPoint = { 0.f, 0.f };
	contactNormal = { 0.f, 0.f };

	// Multiplication is faster than division so we cache it.
	sf::Vector2f invDir = { 1.f / ray.direction.x, 1.f / ray.direction.y };

	sf::Vector2f tNear = 
	{
		(rect.left - ray.origin.x)* invDir.x,
		(rect.top - ray.origin.y) * invDir.y 
	};
	sf::Vector2f tFar =
	{
		(rect.left + rect.width - ray.origin.x) * invDir.x,
		(rect.top + rect.height - ray.origin.y) * invDir.y
	};

	if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
	if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;

	if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
	if (tNear.y < tFar.y) std::swap(tNear.y, tFar.y);

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
		if (invDir.x < 0.f)
			contactNormal = { 0.f, 1.f };
		else
			contactNormal = { 0.f, -1.f };
	}

	return true;
}

bool Physics::dynamicRectThroughRect(const sf::FloatRect& dynRect, const sf::Vector2f& velocity, const sf::FloatRect& statRect, const float& dt, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime)
{
	if (velocity.x == 0.f && velocity.y == 0.f) return false;

	// We have to expand target rectangle by the dimensions of dynamic rectangle.
	sf::FloatRect expandedStatRect =
	{
		sf::Vector2f(statRect.left - dynRect.width / 2.f, statRect.top - dynRect.height / 2.f),
		sf::Vector2f(statRect.width + dynRect.width, statRect.height + dynRect.height)
	};

	if (rayThroughRect
	(
		Ray
		(
			sf::Vector2f(dynRect.left + dynRect.width, dynRect.top + dynRect.height) / 2.f,
			velocity * dt
		),
		expandedStatRect, contactPoint, contactNormal, contactTime
	))
		return (contactTime >= 0.f && contactTime < 1.f);
	else
		return false;
}

bool Physics::resolveDynamicRectWithRect(ColliderComponent* dynRect, sf::Vector2f& velocity, std::array<ColliderComponent*, 4>& contacts, const float& dt, ColliderComponent* statRect)
{
	sf::Vector2f contactPoint, contactNormal;
	float contactTime = 0.f;

	if (dynamicRectThroughRect(dynRect->getHitRect(), velocity, statRect->getHitRect(), dt, contactPoint, contactNormal, contactTime))
	{
		if (contactNormal.y > 0.f) contacts[0] = statRect; else nullptr;
		if (contactNormal.x < 0.f) contacts[0] = statRect; else nullptr;
		if (contactNormal.y < 0.f) contacts[0] = statRect; else nullptr;
		if (contactNormal.x > 0.f) contacts[0] = statRect; else nullptr;

		velocity.x += contactNormal.x * std::abs(velocity.x) * (1 - contactTime);
		velocity.y += contactNormal.y * std::abs(velocity.y) * (1 - contactTime);
		return true;
	}

	return false;
}
