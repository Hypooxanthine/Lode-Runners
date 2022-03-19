#include "LadderTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

LadderTile::LadderTile()
	: LadderTile(false)
{
}

LadderTile::LadderTile(const bool& isExit)
	: m_IsExit(isExit)
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Ladder).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox(m_SpriteComponent->getWorldSize());
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);
}
