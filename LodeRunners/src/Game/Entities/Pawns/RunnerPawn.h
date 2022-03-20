#pragma once

#include "Pawn.h"

class RunnerPawn : public Pawn
{
public:
	RunnerPawn(const size_t& ID, const std::string& name);

	void update(const float& dt) override;

	virtual void onBeginOverlap(Entity* other) override;
	virtual void onEndOverlap(Entity* other) override;

private:
	size_t m_Score = 0;
	size_t m_GoldsTaken = 0;
};