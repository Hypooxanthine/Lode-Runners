#include "RunnerPawn.h"

#include "../../../Assets/Assets.h"
#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"

RunnerPawn::RunnerPawn(const size_t& ID, const std::string& name)
	: Pawn(ID, name)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setRelativePosition({ .25f, 0.f });
	m_Collider->setHitbox({ .5f, 1.f });
}

void RunnerPawn::update(const float& dt)
{
	if (m_IsMovingRight && !m_IsMovingLeft)
	{
		if(m_Flipbook->getType() != FlipbookType::PlayerRight || m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting moving right.");
			m_Flipbook->setType(FlipbookType::PlayerRight);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else if (m_IsMovingLeft && !m_IsMovingRight)
	{
		if(m_Flipbook->getType() != FlipbookType::PlayerLeft || m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting moving left.");
			m_Flipbook->setType(FlipbookType::PlayerLeft);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else
	{
		if(!m_Flipbook->isFrozen())
		{
			LOG_TRACE("Starting idling.");
			m_Flipbook->freeze();
		}
	}

	if (IS_SERVER)
	{
		if (m_IsMovingLeft)
			move({ -dt * m_Speed, 0.f });
		if (m_IsMovingRight)
			move({ dt * m_Speed, 0.f });
		if (m_IsMovingUp)
			move({ 0.f, -dt * m_Speed });
		if (m_IsMovingDown)
			move({ 0.f, dt * m_Speed });
	}
}

void RunnerPawn::render(Ref<sf::RenderWindow> window)
{
}
