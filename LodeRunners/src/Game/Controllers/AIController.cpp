#include "AIController.h"

#include "../Entities/TileMap.h"
#include "../Entities/Tiles/Tiles.h"
#include "../Entities/Pawns/RunnerPawn.h"
#include "../Components/ColliderComponent.h"

const float UPDATE_PATH_TIME = 1.f;

AIController::AIController(TileMap* tileMap, const std::vector<RunnerPawn*>& runners)
	: Controller(), m_TileMap(tileMap), m_Runners(runners), m_TimeAccumulator(UPDATE_PATH_TIME)
{
}

void AIController::update(const float& dt)
{
	GameState* gs = dynamic_cast<GameState*>(Application::get()->getCurrentState());
	if (gs && gs->isGameEnded()) return;

	m_TimeAccumulator += dt;

	if (m_TimeAccumulator >= UPDATE_PATH_TIME)
	{
		m_TimeAccumulator -= UPDATE_PATH_TIME;

		updatePath();
	}

	if(m_ClosestRunner != nullptr)
	{
		updateCursor();
		followPath();
	}
	else
		stopMoving();	
}

bool AIController::updatePath()
{
	m_PathCursor = 0;
	m_Path.clear();

	updateClosestRunner();
	if (m_ClosestRunner == nullptr) return false;

	ColliderComponent* ennemyCollider = nullptr;
	ColliderComponent* runnerCollider = nullptr;

	if (auto tryCollider = getPawn()->getComponent<ColliderComponent>(); tryCollider.has_value()) ennemyCollider = tryCollider.value();
	else return false;
	if (auto tryCollider = m_ClosestRunner->getComponent<ColliderComponent>(); tryCollider.has_value()) runnerCollider = tryCollider.value();
	else return false;

	return AI::ComputeAStar
	(
		m_TileMap->getNavigationGraph(),
		getTileNodePosition(ennemyCollider->getWorldPosition()),
		getTileNodePosition(runnerCollider->getWorldPosition()),
		m_Path
	);
}

void AIController::followPath()
{
	if (m_Path.size() == 0) 
	{
		return;
	}

	if (m_Path.size() == 1) // We want to hit the player (player and AI are on the same tile).
	{
		const auto ennemyPos = getPawn()->getPosition();
		const auto runnerPos = m_ClosestRunner->getPosition();

		if (ennemyPos.x < runnerPos.x)
			setMovingHorizontal(HorizontalMoveDir::Right);
		else if (ennemyPos.x > runnerPos.x)
			setMovingHorizontal(HorizontalMoveDir::Left);
		else
			setMovingVertical(VerticalMoveDir::None);

		if (ennemyPos.y < runnerPos.y)
			setMovingVertical(VerticalMoveDir::Down);
		else if (ennemyPos.y > runnerPos.y)
			setMovingVertical(VerticalMoveDir::Up);
		else
			setMovingHorizontal(HorizontalMoveDir::None);
	}
	else // m_Path.size() > 1, we want to reach next tile in the path.
	{
		const auto ennemyPos = getPawn()->getPosition();
		const auto nextTilePos = getTileWorldPos(m_Path[m_PathCursor + 1].getPosition());

		if (ennemyPos.x < nextTilePos.x)
			setMovingHorizontal(HorizontalMoveDir::Right);
		else if (ennemyPos.x > nextTilePos.x)
			setMovingHorizontal(HorizontalMoveDir::Left);
		else
			setMovingHorizontal(HorizontalMoveDir::None);

		if (ennemyPos.y < nextTilePos.y)
			setMovingVertical(VerticalMoveDir::Down);
		else if (ennemyPos.y > nextTilePos.y)
			setMovingVertical(VerticalMoveDir::Up);
		else
			setMovingVertical(VerticalMoveDir::None);
	}
}

void AIController::updateCursor()
{
	if (m_Path.size() > m_PathCursor + 2)
	{
		const auto& nextNodePos = m_Path[m_PathCursor + 1].getPosition();
		sf::FloatRect tileRect(getTileWorldPos(nextNodePos), sf::Vector2f(1.f, 1.f));
		ColliderComponent* collider = nullptr;

		if (auto tryCollider = getPawn()->getComponent<ColliderComponent>(); tryCollider.has_value())
			collider = tryCollider.value();
		else
			return;

		if(tileRect.contains(collider->getWorldPosition()))
			m_PathCursor++;
	}
}

void AIController::updateClosestRunner()
{
	m_ClosestRunner = nullptr;

	for (auto& runner : m_Runners)
	{
		if (!runner->isKilled())
		{
			if (m_ClosestRunner == nullptr
				|| getPawn()->getSquaredDistanceTo(runner) < getPawn()->getSquaredDistanceTo(m_ClosestRunner))
				m_ClosestRunner = runner;
		}
	}
}

sf::Vector2f AIController::getTileWorldPos(const sf::Vector2i& pos) const
{
	return sf::Vector2f((float)pos.x, (float)pos.y);
}

sf::Vector2i AIController::getTileNodePosition(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)pos.x, (int)pos.y);
}
