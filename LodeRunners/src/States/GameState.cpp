#include "GameState.h"

#include "../Assets/Assets.h"

#include "../Game/Level.h"
#include "../Game/Physics.h"
#include "../Game/Controllers/PlayerController.h"

#include "../HUD/Custom/GameUI.h"

#include "../Network/Network.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, .95f };
const sf::FloatRect HUD_VIEWPORT = { 0.f, .95f, 1.f, .05f };

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

	std::string playerName;
	auto it = std::find_if(runners.begin(), runners.end(), [](const Player& p) { return p.first == PLAYER_ID; });

	if (it == runners.end())
	{
		it = std::find_if(ennemies.begin(), ennemies.end(), [](const Player& p) { return p.first == PLAYER_ID; });

		ASSERT(it != ennemies.end(), "Couldn't find local player ID in players lists.");
	}

	playerName = it->second;

	m_HUD = MakeRef<GameUI>(nullptr, playerName, level->getName());
	m_HUD->setViewport(HUD_VIEWPORT);
}

void GameState::init()
{
	m_HUD->setScore(0);
}

void GameState::update(const float& dt)
{
	m_Level->update(dt);
	m_Physics->update();
	m_HUD->update(dt);
}

void GameState::render(Ref<sf::RenderWindow> window)
{
	m_Level->render(window);
	m_HUD->render(window);
}

void GameState::onResize()
{
	m_Level->onResize();
	m_HUD->onResize();
}

void GameState::setScore(const size_t& score)
{
	m_HUD->setScore(score);
}

void GameState::notifyGoldPicked()
{
	m_Level->notifyGoldPicked();
}

void GameState::notifyRunnerDeath(RunnerPawn* runner)
{
	m_Level->notifyRunnerDeath(runner);
}
