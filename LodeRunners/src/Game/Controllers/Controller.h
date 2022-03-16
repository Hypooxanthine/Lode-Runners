#pragma once

class Pawn;

class Controller
{
public:
	virtual void update(const float& dt) {}

	Pawn* getPawn() { return m_Pawn; }
	void setControlledPawn(Pawn* pawn) { m_Pawn = pawn; }

private:
	Pawn* m_Pawn = nullptr;
};

