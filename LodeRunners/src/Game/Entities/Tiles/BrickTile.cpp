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
	m_Collider->setCollisionProfile(CollisionProfile::TileSolid);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Blocks);
}

void BrickTile::dig()
{
	m_IsDug = true;
	hide();
	LOG_TRACE("Dug !");
}

void BrickTile::hide()
{
	m_SpriteComponent->setSprite(Assets::getTile(TileType::Blank).get());
	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);
}

void BrickTile::show()
{
	m_SpriteComponent->setSprite(Assets::getTile(TileType::Brick).get());
	m_Collider->setCollisionProfile(CollisionProfile::TileSolid);
}
