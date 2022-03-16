#include "RunnerPawn.h"

#include "../../Components/FlipbookComponent.h"
#include "../../../Assets/Assets.h"

RunnerPawn::RunnerPawn(Controller* controller, const size_t& ID)
	: Pawn(controller, ID)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(1.f);
}

void RunnerPawn::update(const float& dt)
{
}

void RunnerPawn::render(Ref<sf::RenderWindow> window)
{
}
