#include "BridgeTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

BridgeTile::BridgeTile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Bridge).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox(m_SpriteComponent->getWorldSize());
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);
}
