#include "EnnemyPawn.h"

#include "../../../Assets/Assets.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/TextComponent.h"

#include "../Tiles/Gold.h"

EnnemyPawn::EnnemyPawn(const size_t& ID, const std::string& name, TileMap* tileMap, const sf::Vector2f& spawnPoint)
	: Pawn(ID, name, tileMap), m_SpawnPoint(spawnPoint)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setRelativePosition({ .25f, 0.f });
	m_Collider->setHitbox({ .5f, 1.f });

	m_Speed = 3.f;
}

void EnnemyPawn::update(const float& dt)
{
	Pawn::update(dt);

	if (m_IsMovingRight && !m_IsMovingLeft)
	{
		if (m_Flipbook->getType() != FlipbookType::EnnemyRight || m_Flipbook->isFrozen())
		{
			m_Flipbook->setType(FlipbookType::EnnemyRight);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else if (m_IsMovingLeft && !m_IsMovingRight)
	{
		if (m_Flipbook->getType() != FlipbookType::EnnemyLeft || m_Flipbook->isFrozen())
		{
			m_Flipbook->setType(FlipbookType::EnnemyLeft);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else
	{
		if (!m_Flipbook->isFrozen())
		{
			m_Flipbook->freeze();
		}
	}
}

void EnnemyPawn::onBeginOverlap(Entity* other)
{
	Pawn::onBeginOverlap(other);

	GoldTile* asGold = dynamic_cast<GoldTile*>(other);

	if (asGold)
	{
		asGold->hide_OnServer();
		m_CarriedGold = asGold;
	}
}

void EnnemyPawn::onEndOverlap(Entity* other)
{
	Pawn::onEndOverlap(other);
}

void EnnemyPawn::respawn()
{
	LOG_TRACE("Respawned !");

	setPositionLocal(m_SpawnPoint);
}
