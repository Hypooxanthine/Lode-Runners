#include "LadderTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

LadderTile::LadderTile()
	: Tile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Ladder).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox({1.f, 1.1f});
	m_Collider->setRelativePosition({ 0.f, -.1f });
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);
}
