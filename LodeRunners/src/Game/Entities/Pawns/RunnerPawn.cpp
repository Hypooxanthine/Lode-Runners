#include "RunnerPawn.h"

#include "../../../Assets/Assets.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/TextComponent.h"

#include "../Tiles/Gold.h"

#include "../../../States/GameState.h"

RunnerPawn::RunnerPawn(const size_t& ID, const std::string& name)
	: Pawn(ID, name)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setRelativePosition({ .25f, 0.f });
	m_Collider->setHitbox({ .5f, 1.f });

	m_Speed = 3.f;
}

void RunnerPawn::update(const float& dt)
{
	Pawn::update(dt);

	if (m_IsMovingRight && !m_IsMovingLeft)
	{
		if (m_Flipbook->getType() != FlipbookType::PlayerRight || m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting moving right.");
			m_Flipbook->setType(FlipbookType::PlayerRight);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else if (m_IsMovingLeft && !m_IsMovingRight)
	{
		if (m_Flipbook->getType() != FlipbookType::PlayerLeft || m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting moving left.");
			m_Flipbook->setType(FlipbookType::PlayerLeft);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else
	{
		if (!m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting idling.");
			m_Flipbook->freeze();
		}
	}
}

void RunnerPawn::onBeginOverlap(Entity* other)
{
	Pawn::onBeginOverlap(other);

	GoldTile* asGold = dynamic_cast<GoldTile*>(other);

	if (asGold)
	{
		asGold->pickup_OnServer();
		m_GoldsTaken++;
		m_Score += GOLD_POINTS;

		auto state = dynamic_cast<GameState*>(Application::get()->getCurrentState());
		if (state) state->setScore(m_Score);

		LOG_INFO("Player {} picked up a gold. New score : {}.", getName(), m_Score);
	}
}

void RunnerPawn::onEndOverlap(Entity* other)
{
	Pawn::onEndOverlap(other);
}
