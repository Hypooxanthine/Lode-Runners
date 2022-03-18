#include "Level.h"

#include "../Assets/Assets.h"

#include "../Network/Network.h"

#include "Entities/Tiles/Tile.h"
#include "Entities/Pawns/RunnerPawn.h"
#include "Entities/Pawns/EnnemyPawn.h"
#include "Controllers/PlayerController.h"

/* CONSTRUCTORS */

Level::Level(Ref<LevelAsset> levelAsset)
	: m_View({0.f, 0.f, SPACE_UNIT * TILES_WIDTH, SPACE_UNIT * TILES_HEIGHT})
{
	initTiles(levelAsset.get());
}

/* PUBLIC MEMBER FUNCTIONS */

void Level::update(const float& dt)
{
	for (auto& t : m_Tiles)
	{
		t->updateComponents(dt);
		t->update(dt);
	}

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
	for(auto& t : m_Tiles)
	{
		t->renderComponents(window);
		t->render(window);
	}

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

void Level::setTile(const size_t& index, const TileType& type)
{

#ifdef _DEBUG
	ASSERT(index < TILES_WIDTH * TILES_HEIGHT, "setTile out of bounds.");
#endif

	m_Tiles[index] = Tile::MakeTile(type);
	m_Tiles[index]->setPosition
	(
		{
			(float)(index % TILES_WIDTH),
			(float)(index / TILES_WIDTH)
		}
	);
}

void Level::setTile(const size_t& x, const size_t& y, const TileType& type)
{
	setTile(y * TILES_WIDTH + x, type);
}

void Level::setViewport(const sf::FloatRect& viewport)
{
	m_View.setViewport(viewport);
}

void Level::addRunner(const Player& runner)
{
	m_Pawns.push_back(MakeRef<RunnerPawn>(runner.first, runner.second));

	if (runner.first == PLAYER_ID)
	{
		ASSERT(m_PlayerController == nullptr, "A game instance can't hold two PlayerControllers !");

		m_PlayerController = MakeRef<PlayerController>();
		m_PlayerController->setControlledPawn(m_Pawns.back().get());
		m_Pawns.back()->setController(m_PlayerController.get());
	}

	m_Pawns.back()->setPosition({ (float)m_RunnerSpawn.x, (float)m_RunnerSpawn.y });
}

void Level::addEnnemy(const Player& ennemy)
{

}

/* PRIVATE MEMBER FUNCTIONS */

void Level::initTiles(const LevelAsset* levelAsset)
{
	for (size_t i = 0; i < levelAsset->getSize(); i++)
	{
		auto& type = levelAsset->at(i)->getType();

		setTile(i, type);

		if (type == TileType::PlayerStart)
			m_RunnerSpawn = { (unsigned)i % TILES_WIDTH, (unsigned)i / TILES_WIDTH };
		else if (type == TileType::EnnemyStart)
			m_EnnemiesSpawns.push_back({ (unsigned)i % TILES_WIDTH, (unsigned)i / TILES_WIDTH });
	}
}
