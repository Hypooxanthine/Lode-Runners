#include "ColliderComponent.h"

ColliderComponent::ColliderComponent()
{
	if(m_ColType == CollisionType::Dynamic)
		m_LastPosition = getParent()->getPosition();

	m_BehaviourWithProfile[CollisionProfile::Tile] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Runner] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Man] = CollisionResponse::Ignore;
}

void ColliderComponent::update(const float& dt)
{
	// Checking if overlaps ended. Returns instantly if "this" isn't overlapping anything.
	for (size_t i = m_OverlappingColliders.size(); i > 0; i--)
	{
		auto& collider = m_OverlappingColliders[i - 1];

		if (!collides(collider) || getBehaviourWith(collider->getCollisionProfile()) != CollisionResponse::Overlaps)
		{
			getParent()->onEndOverlap(collider->getParent());

			// We have to check for other too. If he bothers about overlapping "this", we have to trigger its parent's onEndOverlap event.
			auto it = std::find(collider->m_OverlappingColliders.begin(), collider->m_OverlappingColliders.end(), this);
			if (it != collider->m_OverlappingColliders.end())
			{
				collider->getParent()->onEndOverlap(getParent());
				collider->m_OverlappingColliders.erase(it);
			}

			// Collider is a reference, so we have to erase it at the end.
			m_OverlappingColliders.erase(m_OverlappingColliders.begin() + i - 1);
		}
	}
}

bool ColliderComponent::collides(const ColliderComponent* other) const
{
	if (m_BehaviourWithProfile.at(other->getCollisionProfile()) != CollisionResponse::Ignore)
		return getHitRect().intersects(other->getHitRect());
	else
		return false;
}

bool ColliderComponent::resolveCollisionWith(ColliderComponent* other)
{
	// If there is a collision and other collider doesn't ignore "this"
	if (collides(other) && other->getBehaviourWith(getCollisionProfile()) != CollisionResponse::Ignore)
	{
		// If other collider blocks "this"
		if(other->getBehaviourWith(getCollisionProfile()) == CollisionResponse::Blocks)
		{
			// "this" and other have to clip
			clip(other);
		}
		else if (other->getBehaviourWith(getCollisionProfile()) == CollisionResponse::Overlaps)
		{
			// No collision to resolve, but we must trigger other's onBeginOverlap
			other->getParent()->onBeginOverlap(getParent());
		}

		return true;
	}
	else
		return false;
}

// To be implemented
void ColliderComponent::clip(ColliderComponent* other)
{
	const sf::Vector2f moveDir = getParent()->getPosition() - m_LastPosition;

	// This = dynamic, other = static
	if (other->getCollisionType() == CollisionType::Static)
	{

	}
	// This = dynamic, other = dynamic
	else
	{

	}
}
