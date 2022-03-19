#pragma once

#include "Pawn.h"

class EnnemyPawn : public Pawn
{
public:
	EnnemyPawn(const size_t& ID, const std::string& name);

	void update(const float& dt) override;
};
