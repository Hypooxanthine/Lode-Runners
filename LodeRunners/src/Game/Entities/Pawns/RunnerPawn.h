#pragma once

#include "Pawn.h"

class RunnerPawn : public Pawn
{
public:
	RunnerPawn(Controller* controller, const size_t& ID);

	void update(const float& dt) override;
	void render(Ref<sf::RenderWindow> window) override;
};