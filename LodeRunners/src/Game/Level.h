#pragma once

#include "../Core/Base.h"

#include "../Network/Network.h"

class LevelAsset;

class TileMap;

class Pawn;
class RunnerPawn;
class EnnemyPawn;

class PlayerController;
class AIController;

// first = playerID, second = playerName.
using Player = std::pair<size_t, std::string>;

using TilePosition = sf::Vector2u;

enum class PawnType;

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
	void fillEnnemiesWithIA();

	TileMap* getTileMap() { return m_TileMap.get(); }
	size_t getRunnersNumber() const;

private: // Private members
	Ref<TileMap> m_TileMap;

	// Only one player controller per game instance.
	Ref<PlayerController> m_PlayerController;
	std::vector<Ref<AIController>> m_AIControllers;

	std::vector<Ref<Pawn>> m_Pawns;
	std::vector<RunnerPawn*> m_Runners;
	std::vector<EnnemyPawn*> m_Ennemies;

	std::vector<RunnerPawn*> m_DeadRunners;

	sf::View m_View;
};

