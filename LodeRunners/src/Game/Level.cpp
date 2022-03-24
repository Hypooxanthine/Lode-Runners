#include "Level.h"

#include "../Assets/Assets.h"

#include "../Network/Network.h"

#include "Entities/TileMap.h"
#include "Entities/Pawns/RunnerPawn.h"
#include "Entities/Pawns/EnnemyPawn.h"
#include "Controllers/PlayerController.h"

/* CONSTRUCTORS */

Level::Level(Ref<LevelAsset> levelAsset)
	: m_View({0.f, 0.f, SPACE_UNIT * TILES_WIDTH, SPACE_UNIT * TILES_HEIGHT}),
	  m_TileMap(MakeRef<TileMap>(levelAsset))
{}

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
}

void Level::setViewport(const sf::FloatRect& viewport)
{
	m_View.setViewport(viewport);
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
}

void Level::addEnnemy(const Player& ennemy)
{
	m_Pawns.push_back(MakeRef<EnnemyPawn>(ennemy.first, ennemy.second, m_TileMap.get()));

	if (ennemy.first == PLAYER_ID)
	{
		ASSERT(m_PlayerController == nullptr, "A game instance can't hold two PlayerControllers !");

		m_PlayerController = MakeRef<PlayerController>();
		m_PlayerController->setControlledPawn(m_Pawns.back().get());
		m_Pawns.back()->setController(m_PlayerController.get());
	}

	TilePosition pos = m_TileMap->nextEnnemySpawn();

	m_Pawns.back()->setPositionLocal({ (float)pos.x, (float)pos.y });
}