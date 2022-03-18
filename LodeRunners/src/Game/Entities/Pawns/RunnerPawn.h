#pragma once

#include "Pawn.h"

class RunnerPawn : public Pawn
{
public:
	RunnerPawn(const size_t& ID, const std::string& name);

	void update(const float& dt) override;
	void render(Ref<sf::RenderWindow> window) override;

private: // Private members
	sf::Vector2f m_LastPosition;
};