#pragma once

#include "../Core/Base.h"

#include "../Network/Network.h"

#include "../HUD/Custom/EndGameResultsUI.h"

class LevelAsset;

class TileMap;

class Pawn;
class RunnerPawn;
class PlayerController;

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

	void notifyGoldPicked();
	void notifyRunnerDeath(RunnerPawn* runner);

private: // Private member functions
	void onAllGoldsPicked();
	void onAllRunnersDead();

private: // Private members
	Ref<TileMap> m_TileMap;

	/* Only one player controller per game instance. */
	Ref<PlayerController> m_PlayerController;

	std::vector<Ref<Pawn>> m_Pawns;

	size_t m_RunnersNb = 0;
	std::vector<RunnerPawn*> m_DeadRunners;

	size_t m_GoldsNb = 0;
	size_t m_PickedUpGolds = 0;

	sf::View m_View;

	Ref<EndGameResultsUI> m_EndGameHUD;

private: // Replicated functions

	/* Game end */

	CREATE_REPLICATED_FUNCTION
	(
		onAllGoldsPicked_Multicast,
		[this]() { this->onAllGoldsPicked(); },
		"Level", Network::ReplicationMode::Multicast
	);

	CREATE_REPLICATED_FUNCTION
	(
		onAllRunnersDead_Multicast,
		[this]() { this->onAllRunnersDead(); },
		"Level", Network::ReplicationMode::Multicast
	);

	CREATE_REPLICATED_FUNCTION
	(
		showResults_Multicast,
		[this](const PawnType& winners)
		{
			m_EndGameHUD->show(winners);
		},
		"Level", Network::ReplicationMode::Multicast,
		const PawnType&
	);

	CREATE_REPLICATED_FUNCTION
	(
		backToLobby_Multicast,
		[this]()
		{
			Application::get()->killState();
		},
		"Level", Network::ReplicationMode::Multicast
	);
};

