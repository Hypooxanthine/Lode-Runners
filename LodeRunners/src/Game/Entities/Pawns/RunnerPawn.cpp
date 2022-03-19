#include "RunnerPawn.h"

#include "../../../Assets/Assets.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/TextComponent.h"

RunnerPawn::RunnerPawn(const size_t& ID, const std::string& name)
	: Pawn(ID, name)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setRelativePosition({ .25f, 0.f });
	m_Collider->setHitbox({ .5f, 1.f });

	if (name != "")
		m_NameText->setColor(sf::Color(0x3f85fc));

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

void RunnerPawn::render(Ref<sf::RenderWindow> window)
{
}
