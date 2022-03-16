#pragma once

#include "State.h"

class Level;
class LevelAsset;
class Physics;

class PlayerController;

class GameState : public State
{
public:
	GameState(Ref<LevelAsset> level);

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

private:
	Ref<Physics> m_Physics;

	Ref<Level> m_Level;

	Ref<PlayerController> m_PlayerController;
};

