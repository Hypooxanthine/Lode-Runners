#pragma once

#include "State.h"

class LevelAsset;

class GameState : public State
{
public:
	GameState(Ref<LevelAsset> level);

	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

private:
	Ref<LevelAsset> m_Level;
	sf::View m_LevelView;
};

