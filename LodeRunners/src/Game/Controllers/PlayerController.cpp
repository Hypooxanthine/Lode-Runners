#include "PlayerController.h"

#include "../Entities/Pawns/RunnerPawn.h"

PlayerController::PlayerController()
	: Controller()
{
}

void PlayerController::update(const float& dt)
{
	if (GET_EVENT(EventType::MoveLeft) && !GET_EVENT(EventType::MoveRight))
		setMovingHorizontal(HorizontalMoveDir::Left);
	else if (GET_EVENT(EventType::MoveRight) && !GET_EVENT(EventType::MoveLeft))
		setMovingHorizontal(HorizontalMoveDir::Right);
	else
		setMovingHorizontal(HorizontalMoveDir::None);

	if (GET_EVENT(EventType::MoveUp) && !GET_EVENT(EventType::MoveDown))
		setMovingVertical(VerticalMoveDir::Up);
	else if (GET_EVENT(EventType::MoveDown) && !GET_EVENT(EventType::MoveUp))
		setMovingVertical(VerticalMoveDir::Down);
	else
		setMovingVertical(VerticalMoveDir::None);
}
