#include "PlayerController.h"

#include "../Entities/Pawns/Pawn.h"

void PlayerController::update(const float& dt)
{
	if (GET_EVENT(EventType::MoveLeft) && !m_MoveLeftOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Left, true);
		m_MoveLeftOld = true;
	}
	if (!GET_EVENT(EventType::MoveLeft) && m_MoveLeftOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Left, false);
		m_MoveLeftOld = false;
	}

	if (GET_EVENT(EventType::MoveRight) && !m_MoveRightOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Right, true);
		m_MoveRightOld = true;
	}
	if (!GET_EVENT(EventType::MoveRight) && m_MoveRightOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Right, false);
		m_MoveRightOld = false;
	}

	if (GET_EVENT(EventType::MoveUp) && !m_MoveUpOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Up, true);
		m_MoveUpOld = true;
	}
	if (!GET_EVENT(EventType::MoveUp) && m_MoveUpOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Up, false);
		m_MoveUpOld = false;
	}

	if (GET_EVENT(EventType::MoveDown) && !m_MoveDownOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Down, true);
		m_MoveDownOld = true;
	}
	if (!GET_EVENT(EventType::MoveDown) && m_MoveDownOld)
	{
		getPawn()->setMoving_OnServer(Pawn::MoveDir::Down, false);
		m_MoveDownOld = false;
	}


}
