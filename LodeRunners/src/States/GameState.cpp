#include "GameState.h"

#include "../Assets/Assets.h"
#include "../Game/Level.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, 1.f };

GameState::GameState(Ref<LevelAsset> level)
	: m_Level(MakeRef<Level>(level))
{
	m_Level->setViewport(LEVEL_VIEWPORT);
}

void GameState::update(const float& dt)
{
	m_Level->update(dt);
}

void GameState::render(Ref<sf::RenderWindow> window)
{
	m_Level->render(window);
}

void GameState::onResize()
{
	m_Level->onResize();
}
