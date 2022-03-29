#include "Controller.h"

void Controller::stopMoving()
{
	setMovingHorizontal(HorizontalMoveDir::None);
	setMovingVertical(VerticalMoveDir::None);
}

void Controller::setMovingHorizontal(const HorizontalMoveDir& dir)
{
	if (dir == m_HorizontalDir) return;

	switch (dir)
	{
	case HorizontalMoveDir::None:
		if (m_HorizontalDir == HorizontalMoveDir::Right)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Right, false);
		if (m_HorizontalDir == HorizontalMoveDir::Left)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Left, false);
		break;
	case HorizontalMoveDir::Left:
		if(m_HorizontalDir == HorizontalMoveDir::Right)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Right, false);
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Left, true);
		break;
	case HorizontalMoveDir::Right:
		if (m_HorizontalDir == HorizontalMoveDir::Left)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Left, false);
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Right, true);
		break;
	}

	m_HorizontalDir = dir;
}

void Controller::setMovingVertical(const VerticalMoveDir& dir)
{
	if (dir == m_VerticalDir) return;

	switch (dir)
	{
	case VerticalMoveDir::None:
		if (m_VerticalDir == VerticalMoveDir::Down)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Down, false);
		if (m_VerticalDir == VerticalMoveDir::Up)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Up, false);
		break;
	case VerticalMoveDir::Up:
		if (m_VerticalDir == VerticalMoveDir::Down)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Down, false);
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Up, true);
		break;
	case VerticalMoveDir::Down:
		if (m_VerticalDir == VerticalMoveDir::Up)
			getPawn()->setMoving_OnServer(Pawn::MoveDir::Up, false);
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Down, true);
		break;
	}

	m_VerticalDir = dir;
}

void Controller::setMoving(const Pawn::MoveDir& dir)
{
	switch (dir)
	{
	case Pawn::MoveDir::Left:
		setMovingHorizontal(HorizontalMoveDir::Left);
		break;
	case Pawn::MoveDir::Right:
		setMovingHorizontal(HorizontalMoveDir::Right);
		break;
	case Pawn::MoveDir::Up:
		setMovingVertical(VerticalMoveDir::Up);
		break;
	case Pawn::MoveDir::Down:
		setMovingVertical(VerticalMoveDir::Down);
		break;
	case Pawn::MoveDir::None:
		stopMoving();
		break;
	}
}
