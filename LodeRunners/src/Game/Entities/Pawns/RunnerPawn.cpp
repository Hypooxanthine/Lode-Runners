#include "RunnerPawn.h"

#include "../../../Assets/Assets.h"
#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"

// Because of networking delays
#define ANIM_TIME_FLEXIBILITY .15f

RunnerPawn::RunnerPawn(const size_t& ID, const std::string& name)
	: Pawn(ID, name)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setRelativePosition({ SPACE_UNIT * .25f, 0.f });
	m_Collider->setHitbox({ SPACE_UNIT * .5f, SPACE_UNIT * 1.f });

	m_LastPosition = getPosition();
}

void RunnerPawn::update(const float& dt)
{
	sf::Vector2f movement = getPosition() - m_LastPosition;

	if(m_AnimAccumulator > ANIM_TIME_FLEXIBILITY)
	{
		if (movement.x > 0.f && m_Flipbook->getType() != FlipbookType::PlayerRight)
		{
			m_Flipbook->setType(FlipbookType::PlayerRight);
			m_Flipbook->setCurrentFrame(0);
		}
		else if (movement.x < 0.f && m_Flipbook->getType() != FlipbookType::PlayerLeft)
		{
			m_Flipbook->setType(FlipbookType::PlayerLeft);
			m_Flipbook->setCurrentFrame(0);
		}
		else if (movement.x == 0.f && m_Flipbook->getType() != FlipbookType::PlayerIdle)
		{
			m_Flipbook->setType(FlipbookType::PlayerIdle);
			m_Flipbook->setCurrentFrame(0);
		}

		m_AnimAccumulator -= ANIM_TIME_FLEXIBILITY;
	}

	m_AnimAccumulator += dt;
	m_LastPosition = getPosition();
}

void RunnerPawn::render(Ref<sf::RenderWindow> window)
{
}
