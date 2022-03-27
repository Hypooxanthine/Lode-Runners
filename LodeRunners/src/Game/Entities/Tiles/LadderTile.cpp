#include "LadderTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

LadderTile::LadderTile()
	: Tile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Ladder).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox({1.f, 1.f});
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);

	auto* topCollider = makeComponent<ColliderComponent>("Ladder Top Collider");
	topCollider->setHitbox({ 1.f, 0.f });
	topCollider->setRelativePosition({ 0.f, 0.f });
	topCollider->setCollisionType(CollisionType::Static);
	topCollider->setCollisionProfile(CollisionProfile::LadderTop);
}

void LadderTile::setExit(const bool& val)
{
	m_IsExit = true;
}
