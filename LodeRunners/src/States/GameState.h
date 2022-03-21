#pragma once

#include "State.h"

class Level;
class LevelAsset;
class Physics;

class PlayerController;

class GameUI;

// first = playerID, second = playerName.
using Player = std::pair<size_t, std::string>;

class GameState : public State
{
public:
	GameState(Ref<LevelAsset> level, const std::vector<Player>& runners, const std::vector<Player>& ennemies);

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

	void setScore(const size_t& score);

private:
	Ref<Physics> m_Physics;

	Ref<Level> m_Level;

	Ref<GameUI> m_HUD;
};

