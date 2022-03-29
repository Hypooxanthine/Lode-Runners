#include "AIController.h"

#include "../Entities/TileMap.h"
#include "../Entities/Tiles/Tiles.h"
#include "../Entities/Pawns/RunnerPawn.h"

AIController::AIController(TileMap* tileMap, const std::vector<RunnerPawn*>& runners)
	: Controller(), m_TileMap(tileMap), m_Runners(runners)
{
}

void AIController::update(const float& dt)
{
	m_TimeAccumulator += dt;
	if (m_TimeAccumulator < .5f) return;
	else
		m_TimeAccumulator -= .5f;

	Pawn::MoveDir dir = Pawn::MoveDir::None;

	updateClosestRunner();
	
	if (AI::ComputeAStar(m_TileMap->getNavigationGraph(),
		{ (int)getPawn()->getPosition().x, (int)getPawn()->getPosition().y },
		{ (int)m_ClosestRunner->getPosition().x , (int)m_ClosestRunner->getPosition().y },
		m_Path))
	{
		sf::Vector2i currentPos = { (int)getPawn()->getPosition().x, (int)getPawn()->getPosition().y };

		if(m_Path.size() > 1)
		{
			if      (currentPos.x < m_Path[1].getPosition().x) dir = Pawn::MoveDir::Right;
			else if (currentPos.x > m_Path[1].getPosition().x) dir = Pawn::MoveDir::Left;
			else if (currentPos.y < m_Path[1].getPosition().y) dir = Pawn::MoveDir::Down;
			else if (currentPos.y > m_Path[1].getPosition().y) dir = Pawn::MoveDir::Up;
		}

		setMoving(dir);
	}
	
}

bool AIController::updateClosestRunner()
{
	RunnerPawn* old = m_ClosestRunner;

	for (auto& runner : m_Runners)
	{
		if (!runner->isKilled())
		{
			if (m_ClosestRunner == nullptr
				|| getPawn()->getSquaredDistanceTo(runner) < getPawn()->getSquaredDistanceTo(m_ClosestRunner))
				m_ClosestRunner = runner;
		}
	}

	return m_ClosestRunner != old;
}
