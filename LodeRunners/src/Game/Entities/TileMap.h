#pragma once

#include "Entity.h"

class Tile;
class BlockingWall;
class GoldTile;

using TilePosition = sf::Vector2u;

class TileMap : public Entity
{
public:
	TileMap(Ref<LevelAsset> levelAsset);

	void update(const float& dt) override;
	void render(Ref<sf::RenderWindow> window) override;

	void setTile(const size_t& index, const TileType& type);
	void setTile(const size_t& x, const size_t& y, const TileType& type);

	const Tile* getTile(const size_t& index) const;
	Tile* getTile(const size_t& index);

	TilePosition getRunnersSpawn() const;
	TilePosition nextEnnemySpawn();

private: // Private methods
	void initTiles(const LevelAsset* levelAsset);
	void initBlockingWalls();

private:
	std::array<Ref<Tile>, TILES_HEIGHT* TILES_WIDTH> m_Tiles;
	Ref<BlockingWall> m_WallL, m_WallR, m_WallU, m_WallD;

	TilePosition m_RunnerSpawn;
	std::vector<TilePosition> m_EnnemiesSpawns;
	size_t m_NextEnnemySpawn = 0;

	std::vector<Ref<GoldTile>> m_Golds;
};

