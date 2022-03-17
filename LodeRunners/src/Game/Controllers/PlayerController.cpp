#include "PlayerController.h"

#include "../Entities/Pawns/Pawn.h"

void PlayerController::update(const float& dt)
{
	if (GET_EVENT(EventType::MoveLeft))
	{
		getPawn()->move_OnServer(-dt * 3.f, 0.f);
	}
	if (GET_EVENT(EventType::MoveRight))
	{
		getPawn()->move_OnServer(dt * 3.f, 0.f);
	}
}
