#include "StoneTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

StoneTile::StoneTile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Stone).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox(m_SpriteComponent->getWorldSize());
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::Tile);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Blocks);
}
