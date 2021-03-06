#include "TileMap.h"

#include "Tiles/Tiles.h"
#include "BlockingWall.h"

TileMap::TileMap(Ref<LevelAsset> levelAsset)
{
	initTiles(levelAsset.get());
	initNavigationGraph();
	initBlockingWalls();
}

void TileMap::update(const float& dt)
{
	for (auto& t : m_Tiles)
	{
		t->updateComponents(dt);
		t->update(dt);
	}

	for (auto& g : m_Golds)
	{
		g->updateComponents(dt);
		g->update(dt);
	}
}

void TileMap::render(Ref<sf::RenderWindow> window)
{
	for (auto& t : m_Tiles)
	{
		t->renderComponents(window);
		t->render(window);
	}

	for (auto& g : m_Golds)
	{
		g->renderComponents(window);
		g->render(window);
	}
}

void TileMap::setTile(const size_t& index, const TileType& type)
{
	ASSERT(index < TILES_WIDTH* TILES_HEIGHT, "setTile out of bounds.");

	m_Tiles[index] = Tile::MakeTile(type);
	m_Tiles[index]->setPosition
	(
		{
			(float)(index % TILES_WIDTH),
			(float)(index / TILES_WIDTH)
		}
	);
}

void TileMap::setTile(const size_t& x, const size_t& y, const TileType& type)
{
	setTile(y * TILES_WIDTH + x, type);
}

Tile* TileMap::getTile(const size_t& index)
{
	return m_Tiles[index].get();
}

Tile* TileMap::getTile(const size_t& x, const size_t& y)
{
	return getTile(x + y * TILES_WIDTH);
}

const Data::AStarGraph& TileMap::getNavigationGraph() const
{
	return m_NavGraph;
}

TilePosition TileMap::getRunnersSpawn() const
{
	return m_RunnerSpawn;
}

TilePosition TileMap::nextEnnemySpawn()
{
	return m_EnnemiesSpawns[(m_NextEnnemySpawn++) % m_EnnemiesSpawns.size()];
}

size_t TileMap::getMaxEnnemies() const
{
	return m_EnnemiesSpawns.size();
}

void TileMap::initTiles(const LevelAsset* levelAsset)
{
	for (size_t i = 0; i < levelAsset->getSize(); i++)
	{
		auto& type = levelAsset->at(i)->getType();

		setTile(i, type);

		if (type == TileType::PlayerStart)
			m_RunnerSpawn = { (unsigned)i % TILES_WIDTH, (unsigned)i / TILES_WIDTH };
		else if (type == TileType::EnnemyStart)
			m_EnnemiesSpawns.push_back({ (unsigned)i % TILES_WIDTH, (unsigned)i / TILES_WIDTH });

		if (type == TileType::Gold)
		{
			m_Golds.push_back(MakeRef<GoldTile>(i)); // We can just pass i as ID, there is only one gold per tile.
			m_Golds.back()->setPosition({ (float)(i % TILES_WIDTH), (float)(i / TILES_WIDTH) });
		}
		else if (type == TileType::LevelEnd)
		{
			m_ExitTile = dynamic_cast<ExitTile*>(getTile(i));
		}
	}
}

void TileMap::initNavigationGraph()
{
	for (size_t i = 0; i < TILES_WIDTH * TILES_HEIGHT; i++)
	{
		Tile* tile = getTile(i);

		if (!tile) continue;

		if (dynamic_cast<LadderTile*>(tile) || dynamic_cast<BridgeTile*>(tile)
			|| (tile->isNavigable() && i + TILES_WIDTH < TILES_WIDTH * TILES_HEIGHT
				&& (!getTile(i + TILES_WIDTH)->isNavigable() || dynamic_cast<LadderTile*>(getTile(i + TILES_WIDTH)))))
		{
			sf::Vector2i pos = { (int)i % TILES_WIDTH, (int)i / TILES_WIDTH };
			m_NavGraph.addNode(pos);

			if (m_NavGraph.contains({ pos.x - 1, pos.y }))
			{
				m_NavGraph.addEdge(pos, { pos.x - 1, pos.y });
				m_NavGraph.addEdge({ pos.x - 1, pos.y }, pos);
			}
			if (m_NavGraph.contains({ pos.x, pos.y - 1 }))
			{
				m_NavGraph.addEdge(pos, { pos.x, pos.y - 1 });
				m_NavGraph.addEdge({ pos.x, pos.y - 1 }, pos);
			}
		}
	}
}

void TileMap::initBlockingWalls()
{
	m_WallL = MakeRef<BlockingWall>();
	m_WallL->setPosition({ -1.f, 0.f });
	m_WallL->setSize({ 1.f, (float)TILES_HEIGHT });

	m_WallR = MakeRef<BlockingWall>();
	m_WallR->setPosition({ (float)TILES_WIDTH, 0.f });
	m_WallR->setSize({ 1.f, (float)TILES_HEIGHT });

	m_WallU = MakeRef<BlockingWall>();
	m_WallU->setPosition({ 0.f, -1.f });
	m_WallU->setSize({ (float)TILES_WIDTH, 1.f });

	m_WallD = MakeRef<BlockingWall>();
	m_WallD->setPosition({ 0.f, (float)TILES_HEIGHT });
	m_WallD->setSize({ (float)TILES_WIDTH, 1.f });
}