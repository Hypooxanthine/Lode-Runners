#pragma once

#include "../../Core/Base.h"

#include "../Entities/Pawns/Pawn.h"

class Controller
{
protected:
	enum class HorizontalMoveDir { Left, Right, None};
	enum class VerticalMoveDir { Up, Down, None };
public:
	virtual void update(const float& dt) {}

	Pawn* getPawn() { return m_Pawn; }
	void setControlledPawn(Pawn* pawn) { m_Pawn = pawn; }

protected:
	void stopMoving();

	void setMovingHorizontal(const HorizontalMoveDir& dir);
	void setMovingVertical(const VerticalMoveDir& dir);

	void setMoving(const Pawn::MoveDir& dir);

protected:
	HorizontalMoveDir m_HorizontalDir = HorizontalMoveDir::None;
	VerticalMoveDir m_VerticalDir = VerticalMoveDir::None;

private:
	Pawn* m_Pawn = nullptr;
};

