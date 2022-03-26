#include "Level.h"

#include "../Assets/Assets.h"

#include "../Network/Network.h"

#include "Entities/TileMap.h"
#include "Entities/Pawns/RunnerPawn.h"
#include "Entities/Pawns/EnnemyPawn.h"

#include "Entities/Tiles/ExitTile.h"
#include "Entities/Tiles/LadderTile.h"

#include "Controllers/PlayerController.h"

/* CONSTRUCTORS */

Level::Level(Ref<LevelAsset> levelAsset)
	: m_View({0.f, 0.f, SPACE_UNIT * TILES_WIDTH, SPACE_UNIT * TILES_HEIGHT}),
	  m_TileMap(MakeRef<TileMap>(levelAsset)), m_GoldsNb(m_TileMap->getGoldsNb()),
	m_EndGameHUD(MakeRef<EndGameResultsUI>())
{
	m_EndGameHUD->bindEvent([this] {this->backToLobby_Multicast(); });
}

/* PUBLIC MEMBER FUNCTIONS */

void Level::update(const float& dt)
{
	m_TileMap->update(dt);

	for (auto& p : m_Pawns)
	{
		p->updateComponents(dt);
		p->update(dt);
	}

	m_PlayerController->update(dt);
	m_EndGameHUD->update(dt);
}

void Level::render(Ref<sf::RenderWindow> window)
{
	window->setView(m_View);

	m_TileMap->render(window);

	for (auto& p : m_Pawns)
	{
		p->renderComponents(window);
		p->render(window);
	}

	m_EndGameHUD->render(window);
}

void Level::onResize()
{
	const sf::FloatRect viewportRect = (sf::FloatRect)Application::get()->getWindow()->getViewport(m_View);
	const sf::Vector2f viewportSize =
	{
		viewportRect.width,
		viewportRect.height
	};
	const sf::Vector2f levelSize =
	{
		(float)(TILES_WIDTH * Assets::getElementSize()),
		(float)(TILES_HEIGHT * Assets::getElementSize())
	};

	const float levelRatio = levelSize.x / levelSize.y;
	const float viewportRatio = viewportSize.x / viewportSize.y;

	if (levelRatio > viewportRatio)
	{
		const float heightCorrection = levelSize.x * viewportSize.y / viewportSize.x - levelSize.y;

		m_View.setSize(levelSize.x, levelSize.y + heightCorrection);
	}
	else
	{
		const float widthCorrection = viewportSize.x * levelSize.y / viewportSize.y - levelSize.x;

		m_View.setSize(levelSize.x + widthCorrection, levelSize.y);
	}

	m_EndGameHUD->onResize();
}

void Level::setViewport(const sf::FloatRect& viewport)
{
	m_View.setViewport(viewport);
	m_EndGameHUD->setViewport(viewport);
}

void Level::addRunner(const Player& runner)
{
	m_Pawns.push_back(MakeRef<RunnerPawn>(runner.first, runner.second, m_TileMap.get()));

	if (runner.first == PLAYER_ID)
	{
		ASSERT(m_PlayerController == nullptr, "A game instance can't hold two PlayerControllers !");

		m_PlayerController = MakeRef<PlayerController>();
		m_PlayerController->setControlledPawn(m_Pawns.back().get());
		m_Pawns.back()->setController(m_PlayerController.get());
	}

	TilePosition pos = m_TileMap->getRunnersSpawn();

	m_Pawns.back()->setPositionLocal({ (float)pos.x, (float)pos.y });
	m_RunnersNb++;
}

void Level::addEnnemy(const Player& ennemy)
{
	auto spawnPoint = (sf::Vector2f)m_TileMap->nextEnnemySpawn();

	m_Pawns.push_back(MakeRef<EnnemyPawn>(ennemy.first, ennemy.second, m_TileMap.get(), spawnPoint));

	if (ennemy.first == PLAYER_ID)
	{
		ASSERT(m_PlayerController == nullptr, "A game instance can't hold two PlayerControllers !");

		m_PlayerController = MakeRef<PlayerController>();
		m_PlayerController->setControlledPawn(m_Pawns.back().get());
		m_Pawns.back()->setController(m_PlayerController.get());
	}

	m_Pawns.back()->setPositionLocal({ spawnPoint.x, spawnPoint.y });
}

void Level::notifyGoldPicked()
{
	m_PickedUpGolds++;

	if (m_PickedUpGolds == m_GoldsNb)
		onAllGoldsPicked_Multicast();
}

void Level::notifyRunnerDeath(RunnerPawn* runner)
{
	if (std::find(m_DeadRunners.begin(), m_DeadRunners.end(), runner) != m_DeadRunners.end())
		return; // Pawn already registered as dead.

	m_DeadRunners.push_back(runner);

	// If all runners are dead
	if (m_DeadRunners.size() == m_RunnersNb)
		onAllRunnersDead_Multicast();
}

/* PRIVATE MEMBER FUNCTIONS */

void Level::onAllGoldsPicked()
{
	const auto exitTileMapPos = m_TileMap->getExitTile()->getTileMapPosition();

	m_TileMap->setTile(exitTileMapPos.x, exitTileMapPos.y, TileType::Ladder);
	dynamic_cast<LadderTile*>(m_TileMap->getTile(exitTileMapPos.x, exitTileMapPos.y))->setExit();

	// We want to create ladders on the exit tile but also on every blank tiles below.
	auto isBlankTile = [this](const sf::Vector2u& pos) -> bool
	{
		if (pos.x >= TILES_WIDTH || pos.y >= TILES_HEIGHT) return false;

		return dynamic_cast<BlankTile*>(m_TileMap->getTile(size_t(pos.x + pos.y * TILES_WIDTH))) != nullptr;
	};

	for (sf::Vector2u cursor(exitTileMapPos.x, exitTileMapPos.y + 1); isBlankTile(cursor); cursor.y++)
	{
		m_TileMap->setTile(cursor.x, cursor.y, TileType::Ladder);
	}

	if (IS_SERVER) showResults_Multicast(PawnType::Runner);
}

void Level::onAllRunnersDead()
{
	if (IS_SERVER) showResults_Multicast(PawnType::Ennemy);
}
