#include "BrickTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

BrickTile::BrickTile()
	: Tile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Brick).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox({ 1.f, 1.f });
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::Tile);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Blocks);
}
