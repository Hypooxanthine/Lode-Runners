#include "ColliderComponent.h"

#include "../Physics.h"

ColliderComponent::ColliderComponent()
{
	if(m_ColType == CollisionType::Dynamic)
		m_LastPosition = getParent()->getPosition();

	m_BehaviourWithProfile[CollisionProfile::Tile] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Runner] = CollisionResponse::Ignore;
	m_BehaviourWithProfile[CollisionProfile::Man] = CollisionResponse::Ignore;

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

		if (!this->collides(collider) || this->getBehaviourWith(collider->getCollisionProfile()) != CollisionResponse::Overlaps)
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

bool ColliderComponent::collides(const ColliderComponent* other) const
{
	if (m_BehaviourWithProfile.at(other->getCollisionProfile()) != CollisionResponse::Ignore)
		return getHitRect().intersects(other->getHitRect());
	else
		return false;
}

bool ColliderComponent::resolveCollisionWith(ColliderComponent* other)
{
	if(this->collides(other))
	{
		// If "this" blocks "other"
		if (this->getBehaviourWith(other->getCollisionProfile()) == CollisionResponse::Blocks)
		{
			// "other" and "this" have to clip
			clip(other);
			return true;
		}
		// If "this" overlaps "other"
		else if (this->getBehaviourWith(other->getCollisionProfile()) == CollisionResponse::Overlaps)
		{
			// No collision to resolve, but we must trigger onBeginOverlap
			this->getParent()->onBeginOverlap(other->getParent());
			m_OverlappingColliders.push_back(other);
			return true;
		}
		else
			return false;
	}
}

// To be implemented
void ColliderComponent::clip(ColliderComponent* other)
{
	/* At this point we know that "this" blocks "other", and that a collision occured. The wording isn't really correct :
	 * in fact, this means  that "this" can't overlap "other". So we should say that "other" blocks "this". But this is
	 * easier to store this response in "this", with the other ones.
	 */

	const sf::Vector2f moveDir = getParent()->getPosition() - m_LastPosition;

	if(this->getCollisionType() == CollisionType::Static)
	{
		/*
		 *  if (other->getCollisionType() == CollisionType::Static)
		 *  {
		 *  	// Shouldn't happen because the colliders should both be "static" <=> they never move.
		 *  	// It could happen when to static colliders were spawned at the same place.
		 *  	// Not doing anything is fine, because it allows to place multiple colliders at the same
		 *  	// place whitout causing any collision issue.
		 *  	// By the way : a tile is static, and should response "Ignore" to another tile.
		 *  	// It should only block pawns.
		 *  }
		 *  else
		 *  {
		 *  	// Not really important. "this" is static, and blocks "other" <=> "this" can't overlap "other".
		 *  	// But "this" being static, it should never try to overlap anything ("this" is not moving).
		 *  }
		 */
	}
	// Here comes the fun
	else
	{
		// At this point, "this" is dynamic.

		if (other->getCollisionType() == CollisionType::Static)
		{
			/* 
			 * "this" is dynamic and "other" is static.
			 * In order to resolve collisions, we need to clip "this" only, and let "other" where it is.
			 */


		}
		else
		{
			/* 
			 * "this" is dynamic and "other" is dynamic.
			 * In order to resolve collisions, we need to clip both enties together.
			 * We need to sum the movement vectors, and move both of the entities, because they are
			 * pushing themselves together.
			 */


		}
	}
}
