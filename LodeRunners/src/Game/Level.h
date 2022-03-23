#pragma once

#include "../Core/Base.h"

class LevelAsset;

class TileMap;

class Pawn;
class PlayerController;

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

	// Values between 0 and 1 for the edges of the window.
	void setViewport(const sf::FloatRect& viewport);

	void addRunner(const Player& runner);
	void addEnnemy(const Player& ennemy);

private: // Private members
	Ref<TileMap> m_TileMap;

	/* Only one player controller per instance. */
	Ref<PlayerController> m_PlayerController;
	std::vector<Ref<Pawn>> m_Pawns;

	sf::View m_View;
};

