#pragma once

class Pawn;

class Controller
{
public:
	Controller(Pawn* controld) = delete;

	virtual void update(const float& dt) {}

	Pawn* getPawn() { return m_Pawn; }

private:
	Pawn* m_Pawn;
};

