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
	bool updateClosestRunner();

private:
	TileMap* m_TileMap;
	std::vector<RunnerPawn*> m_Runners;

	std::vector<Data::Node> m_Path;
	RunnerPawn* m_ClosestRunner = nullptr;

	float m_TimeAccumulator = 0.f;

	float m_Speed = 3.f;
};

