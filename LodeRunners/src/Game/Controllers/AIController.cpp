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

	updateClosestRunner();

	if (!m_ClosestRunner->isKilled()/*m_TimeAccumulator > .1f*/)
	{
		m_TimeAccumulator -= .1f;

		if (AI::ComputeAStar(m_TileMap->getNavigationGraph(),
			{ (int)getPawn()->getPosition().x, (int)getPawn()->getPosition().y },
			{ (int)m_ClosestRunner->getPosition().x , (int)m_ClosestRunner->getPosition().y },
			m_Path))
		{
			m_PathCursor = 0;
		}

		sf::Vector2i currentPos = { (int)getPawn()->getPosition().x, (int)getPawn()->getPosition().y };

		if (m_Path.size() > 1)
		{
			if (currentPos.x < m_Path[m_PathCursor + 1].getPosition().x) 
			{
				setMovingVertical(VerticalMoveDir::None);
				setMovingHorizontal(HorizontalMoveDir::Right);
			}
			else if (currentPos.x > m_Path[m_PathCursor + 1].getPosition().x) 
			{
				setMovingVertical(VerticalMoveDir::None);
				setMovingHorizontal(HorizontalMoveDir::Left);
			}
			else if (currentPos.y < m_Path[m_PathCursor + 1].getPosition().y) 
			{
				setMovingHorizontal(HorizontalMoveDir::None);
				setMovingVertical(VerticalMoveDir::Down);
			}
			else if (currentPos.y > m_Path[m_PathCursor + 1].getPosition().y) 
			{
				setMovingHorizontal(HorizontalMoveDir::None);
				setMovingVertical(VerticalMoveDir::Up);
			}
		}
	}
	else
		stopMoving();
	
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
