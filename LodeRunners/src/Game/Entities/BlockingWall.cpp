#include "BlockingWall.h"

#include "../Components/ColliderComponent.h"

BlockingWall::BlockingWall()
{
	m_Collider = makeComponent<ColliderComponent>("Collider");

	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileSolid);
	m_Collider->setRelativePosition({ 0.f, 0.f });

	setSize({ 1.f, 1.f });
}

void BlockingWall::setSize(const sf::Vector2f& size)
{
	m_Collider->setHitbox(size);
}
