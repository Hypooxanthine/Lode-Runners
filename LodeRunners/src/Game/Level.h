#pragma once

#include "../Core/Base.h"

class LevelAsset;
enum class TileType;

class Tile;
class Pawn;
class PlayerController;
class BlockingWall;
class GoldTile;

// first = playerID, second = playerName.
using Player = std::pair<size_t, std::string>;

using TilePosition = sf::Vector2u;

class Level
{
public:
	Level(Ref<LevelAsset> levelAsset);

	void update(const float& dt);
	void render(Ref<sf::RenderWindow> window);
	void onResize();

	void setTile(const size_t& index, const TileType& type);
	void setTile(const size_t& x, const size_t& y, const TileType& type);

	// Values between 0 and 1 for the edges of the window.
	void setViewport(const sf::FloatRect& viewport);

	void addRunner(const Player& runner);
	void addEnnemy(const Player& ennemy);

private: // Private methods
	void initTiles(const LevelAsset* levelAsset);
	void initBlockingWalls();

private: // Private members
	/* Level resources */
	std::array<Ref<Tile>, TILES_HEIGHT * TILES_WIDTH> m_Tiles;
	Ref<BlockingWall> m_WallL, m_WallR, m_WallU, m_WallD;

	TilePosition m_RunnerSpawn;
	std::vector<TilePosition> m_EnnemiesSpawns;

	std::vector<Ref<GoldTile>> m_Golds;

	/* Only one player controller per instance. */
	Ref<PlayerController> m_PlayerController;
	std::vector<Ref<Pawn>> m_Pawns;

	sf::View m_View;
};

