#pragma once

#include "../Core/Base.h"
#include "../Assets/LevelAsset.h"
#include "Entities/Tiles/Tile.h"

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

private: // Private methods
	void initTiles(const LevelAsset* levelAsset);

private: // Private members
	std::array<Ref<Tile>, TILES_HEIGHT * TILES_WIDTH> m_Tiles;
	sf::View m_View;
};

