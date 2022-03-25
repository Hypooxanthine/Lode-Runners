#include "BrickTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../../Assets/Assets.h"

#include "../Pawns/RunnerPawn.h"
#include "../Pawns/EnnemyPawn.h"

const float REFILL_TIME = 5.f;

BrickTile::BrickTile(const size_t& ID)
	: Tile(), m_ID(ID)
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Brick).get());

	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setHitbox({ 1.f, 1.f });
	m_Collider->setCollisionType(CollisionType::Static);
	m_Collider->setCollisionProfile(CollisionProfile::TileSolid);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Blocks);
}

void BrickTile::update(const float& dt)
{
	// Server only behaviour
	if (!m_IsDug || !IS_SERVER) return;

	m_TimeAccumulator += dt;

	if (m_TimeAccumulator > REFILL_TIME)
		refill_Multicast();
}

void BrickTile::dig()
{
	if (m_IsDug) return;

	hide();

	m_IsDug = true;
	m_TimeAccumulator = 0.f;

	LOG_TRACE("Dug !");
}

void BrickTile::notifyPawnOverlap(Pawn* p)
{
	// Should be called from server (overlapping is physics and is handled by server).
	m_OverlappingPawns.push_back(p);
}

void BrickTile::notifyPawnEndOverlap(Pawn* p)
{
	// Same as notifyPawnOverlap(Pawn*).
	auto it = std::find(m_OverlappingPawns.begin(), m_OverlappingPawns.end(), p);

	if (it != m_OverlappingPawns.end()) m_OverlappingPawns.erase(it);
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

void BrickTile::refill()
{
	show();
	m_IsDug = false;

	if (!IS_SERVER) return;

	// This has no point on clients. Overlapping is handled by server.
	for (size_t i = m_OverlappingPawns.size(); i > 0; i--)
	{
		auto& ref = m_OverlappingPawns[i - 1];

		RunnerPawn* asRunner = dynamic_cast<RunnerPawn*>(ref);

		if (asRunner)
		{
			asRunner->kill_Multicast();
		}
		else
		{
			EnnemyPawn* asEnnemy = dynamic_cast<EnnemyPawn*>(ref);

			if (asEnnemy)
				asEnnemy->respawn_Multicast();
		}
	}
}
