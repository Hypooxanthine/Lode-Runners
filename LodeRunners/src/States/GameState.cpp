#include "GameState.h"

#include "../Assets/Assets.h"

#include "../Game/Level.h"
#include "../Game/Physics.h"
#include "../Game/Controllers/PlayerController.h"

#include "../Game/Entities/TileMap.h"
#include "../Game/Entities/Tiles/ExitTile.h"
#include "../Game/Entities/Tiles/LadderTile.h"

#include "../HUD/Custom/GameUI.h"

#include "../Network/Network.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, .95f };
const sf::FloatRect HUD_VIEWPORT = { 0.f, .95f, 1.f, .05f };

GameState::GameState(Ref<LevelAsset> level, const std::vector<Player>& runners, const std::vector<Player>& ennemies)
	: m_Physics(MakeRef<Physics>()), m_Level(MakeRef<Level>(level)),
	  m_EndGameHUD(MakeRef<EndGameResultsUI>()),
	  m_GoldsNb(m_Level->getTileMap()->getGoldsNb()), m_RunnersNb(runners.size())
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

	m_GameUI = MakeRef<GameUI>(nullptr, playerName, level->getName());
	m_GameUI->setViewport(HUD_VIEWPORT);

	m_EndGameHUD->setViewport(LEVEL_VIEWPORT);
	m_EndGameHUD->bindEvent([this] {this->backToLobby_Multicast(); });
}

void GameState::init()
{
	m_GameUI->setScore(0);
}

void GameState::update(const float& dt)
{
	m_Level->update(dt);
	m_Physics->update();
	m_GameUI->update(dt);
	m_EndGameHUD->update(dt);
}

void GameState::render(Ref<sf::RenderWindow> window)
{
	m_Level->render(window);
	m_GameUI->render(window);
	m_EndGameHUD->render(window);
}

void GameState::onResize()
{
	m_Level->onResize();
	m_GameUI->onResize();
	m_EndGameHUD->onResize();
}

void GameState::setScore(const size_t& score)
{
	m_GameUI->setScore(score);
}

void GameState::notifyGoldPicked()
{
	if (!IS_SERVER) return;

	m_PickedUpGolds++;

	if (m_PickedUpGolds == m_GoldsNb)
		onAllGoldsPicked_Multicast();
}

void GameState::notifyRunnerDeath()
{
	if (!IS_SERVER) return;

	m_DeadRunnersNb++;

	// If all runners are dead
	if (m_DeadRunnersNb == m_RunnersNb)
		endGame_Multicast(PawnType::Ennemy);
}

void GameState::onAllGoldsPicked()
{
	TileMap* tileMap = m_Level->getTileMap();
	const auto exitTileMapPos = tileMap->getExitTile()->getTileMapPosition();

	tileMap->setTile(exitTileMapPos.x, exitTileMapPos.y, TileType::Ladder);
	dynamic_cast<LadderTile*>(tileMap->getTile(exitTileMapPos.x, exitTileMapPos.y))->setExit();

	// We want to create ladders on the exit tile but also on every blank tiles below.
	auto isBlankTile = [&, this](const sf::Vector2u& pos) -> bool
	{
		if (pos.x >= TILES_WIDTH || pos.y >= TILES_HEIGHT) return false;

		return dynamic_cast<BlankTile*>(tileMap->getTile(size_t(pos.x + pos.y * TILES_WIDTH))) != nullptr;
	};

	for (sf::Vector2u cursor(exitTileMapPos.x, exitTileMapPos.y + 1); isBlankTile(cursor); cursor.y++)
	{
		tileMap->setTile(cursor.x, cursor.y, TileType::Ladder);
	}
}