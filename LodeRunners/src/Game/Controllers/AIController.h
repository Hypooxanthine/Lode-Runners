#pragma once

#include "Controller.h"

#include "../../Util/Util.h"

class TileMap;
class RunnerPawn;

class AIController : public Controller
{
public:
	AIController(TileMap* tileMap, const std::vector<RunnerPawn*>& runners);

	virtual void update(const float& dt) override;

private:
	bool updatePath();
	void followPath();
	void updateCursor();

	void updateClosestRunner();
	sf::Vector2f getTileWorldPos(const sf::Vector2i& pos) const;
	sf::Vector2i getTileNodePosition(const sf::Vector2f& pos) const;

private:
	TileMap* m_TileMap;
	std::vector<RunnerPawn*> m_Runners;

	std::vector<Data::Node> m_Path;
	size_t m_PathCursor = 0;
	RunnerPawn* m_ClosestRunner = nullptr;

	float m_TimeAccumulator = 0.f;

	float m_Speed = 3.f;
};

