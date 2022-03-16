#include "GameState.h"

#include "../Assets/Assets.h"
#include "../Game/Level.h"
#include "../Game/Physics.h"
#include "../Game/Controllers/PlayerController.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, 1.f };

GameState::GameState(Ref<LevelAsset> level)
	: m_Physics(MakeRef<Physics>()), m_Level(MakeRef<Level>(level)), m_PlayerController(MakeRef<PlayerController>())
{
	m_Level->setViewport(LEVEL_VIEWPORT);
}

void GameState::init()
{

}

void GameState::update(const float& dt)
{
	m_PlayerController->update(dt);
	m_Level->update(dt);

	m_Physics->update();
}

void GameState::render(Ref<sf::RenderWindow> window)
{
	m_Level->render(window);
}

void GameState::onResize()
{
	m_Level->onResize();
}
