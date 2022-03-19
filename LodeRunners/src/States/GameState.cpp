#include "GameState.h"

#include "../Assets/Assets.h"
#include "../Game/Level.h"
#include "../Game/Physics.h"
#include "../Game/Controllers/PlayerController.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, 1.f };

GameState::GameState(Ref<LevelAsset> level, const std::vector<Player>& runners, const std::vector<Player>& ennemies)
	: m_Physics(MakeRef<Physics>()), m_Level(MakeRef<Level>(level))
{
	m_Level->setViewport(LEVEL_VIEWPORT);

	for (auto& p : runners)
	{
		m_Level->addRunner(p);
	}

	for (auto& p : ennemies)
	{
		m_Level->addEnnemy(p);
	}
}

void GameState::init()
{

}

void GameState::update(const float& dt)
{
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
