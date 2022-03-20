#include "Gold.h"

#include "../../../Assets/Assets.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"

GoldTile::GoldTile(const size_t& ID)
	: Tile(), m_ID(ID)
{
	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setHitbox({ 1.f, 1.f });

	show();
}

void GoldTile::pickUp()
{
	hide();
	m_PickedUp = true;
}

void GoldTile::hide()
{
	removeComponent("Sprite");
	m_Collider->setCollisionProfile(CollisionProfile::NoCollision);

	m_Sprite = nullptr;
	m_Hidden = true;
}

void GoldTile::show()
{
	m_Sprite = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Gold).get());

	m_Collider->setCollisionProfile(CollisionProfile::TileTransparent);

	m_Hidden = false;
}

const bool& GoldTile::wasTaken() const
{
	return m_PickedUp;
}
