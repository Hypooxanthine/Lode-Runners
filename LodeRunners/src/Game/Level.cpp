#include "Level.h"
#include "Entities/Tiles/Tile.h"

/* CONSTRUCTORS */

Level::Level(Ref<LevelAsset> levelAsset)
	: m_View({0.f, 0.f, (float)Assets::getElementSize() * TILES_WIDTH, (float)Assets::getElementSize() * TILES_HEIGHT})
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
}

void Level::render(Ref<sf::RenderWindow> window)
{
	window->setView(m_View);
	for(auto& t : m_Tiles)
	{
		t->renderComponents(window);
		t->render(window);
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
			(float)Assets::getElementSize() * (index % TILES_WIDTH),
			(float)Assets::getElementSize() * (index / TILES_WIDTH)
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

/* PRIVATE MEMBER FUNCTIONS */

void Level::initTiles(const LevelAsset* levelAsset)
{
	for (size_t i = 0; i < levelAsset->getSize(); i++)
		setTile(i, levelAsset->at(i)->getType());
}
