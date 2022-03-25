#include "ColliderComponent.h"

#include "../Physics.h"
#include "../../Assets/Assets.h"

ColliderComponent::ColliderComponent()
{
	if(m_ColType == CollisionType::Dynamic)
		m_LastPosition = getWorldPosition();

	m_BehaviourWithProfile[CollisionProfile::TileSolid] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::TileTransparent] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::NoCollision] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Runner] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Ennemy] = CollisionResponse::Ignore;

	Physics::get()->registerCollider(this);
}

ColliderComponent::~ColliderComponent()
{
	Physics::get()->unregisterCollider(this);
}

void ColliderComponent::update(const float& dt)
{
	// Checking if overlaps ended. Returns instantly if "this" isn't overlapping anything.
	for (size_t i = m_OverlappingColliders.size(); i > 0; i--)
	{
		auto& collider = m_OverlappingColliders[i - 1];

		if (!getHitRect().intersects(collider->getHitRect()) || this->getBehaviourWith(collider->getCollisionProfile()) != CollisionResponse::Overlaps)
		{
			this->getParent()->onEndOverlap(collider->getParent());

			// We have to check for other too. If he bothers about overlapping "this", we have to trigger its parent's onEndOverlap event.
			auto it = std::find(collider->m_OverlappingColliders.begin(), collider->m_OverlappingColliders.end(), this);
			if (it != collider->m_OverlappingColliders.end())
			{
				collider->getParent()->onEndOverlap(this->getParent());
				collider->m_OverlappingColliders.erase(it);
			}

			// Collider is a reference, so we have to erase it at the end.
			m_OverlappingColliders.erase(m_OverlappingColliders.begin() + i - 1);
		}
	}
}

void ColliderComponent::render(Ref<sf::RenderWindow> window)
{
	/* Uncomment to see debug collider shapes. */
	//sf::RectangleShape shape;
	//shape.setPosition(getWorldPosition() * SPACE_UNIT);
	//shape.setSize(getHitbox() * SPACE_UNIT);
	//shape.setOutlineThickness(-1.f);
	//shape.setFillColor(sf::Color::Transparent);
	//shape.setOutlineColor(sf::Color::Red);
	//
	//window->draw(shape);
}

sf::Vector2f ColliderComponent::getHitbox() const
{
	return m_Hitbox;
}

void ColliderComponent::setHitbox(const sf::Vector2f& size)
{
	m_Hitbox = size;
}

sf::FloatRect ColliderComponent::getHitRect() const
{
	return sf::FloatRect(getWorldPosition(), m_Hitbox);
}

const CollisionType& ColliderComponent::getCollisionType() const
{
	return m_ColType;
}

void ColliderComponent::setCollisionType(const CollisionType& type)
{
	m_ColType = type;
	Physics::get()->updateColliderType(this);
}

const CollisionProfile& ColliderComponent::getCollisionProfile() const
{
	return m_ColProfile;
}

void ColliderComponent::setCollisionProfile(const CollisionProfile& profile)
{
	m_ColProfile = profile;
}

const CollisionResponse& ColliderComponent::getBehaviourWith(const CollisionProfile& profile) const
{
	return m_BehaviourWithProfile.at(profile);
}

void ColliderComponent::setBehavioursWith(const CollisionProfile& profile, const CollisionResponse& response)
{
	m_BehaviourWithProfile[profile] = response;
}

bool ColliderComponent::ignores(const ColliderComponent* other) const
{
	return getBehaviourWith(other->getCollisionProfile()) == CollisionResponse::Ignore;
}

bool ColliderComponent::overlaps(const ColliderComponent* other) const
{
	return getBehaviourWith(other->getCollisionProfile()) == CollisionResponse::Overlaps;
}

bool ColliderComponent::blocks(const ColliderComponent* other) const
{
	return getBehaviourWith(other->getCollisionProfile()) == CollisionResponse::Blocks;
}

void ColliderComponent::notifyOverlaps(ColliderComponent* other)
{
	m_OverlappingColliders.push_back(other);
	getParent()->onBeginOverlap(other->getParent());
}

sf::Vector2f ColliderComponent::getLastPosition() const
{
	return m_LastPosition;
}

void ColliderComponent::setLastPosition(const sf::Vector2f& position)
{
	m_LastPosition = position;
}
