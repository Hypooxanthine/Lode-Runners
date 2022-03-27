#pragma once

#include "State.h"

#include "../HUD/Custom/EndGameResultsUI.h"

#include "../Network/Network.h"

class Level;
class LevelAsset;
class Physics;

class PlayerController;
class RunnerPawn;

class GameUI;

// first = playerID, second = playerName.
using Player = std::pair<size_t, std::string>;

class GameState : public State
{
public:
	GameState(Ref<LevelAsset> level, const std::vector<Player>& runners, const std::vector<Player>& ennemies);

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

	void setScore(const size_t& score);

	void notifyGoldPicked();
	void notifyRunnerDeath();

private: // Private member functions
	void onAllGoldsPicked();

private:
	Ref<Physics> m_Physics;

	Ref<Level> m_Level;

	Ref<GameUI> m_GameUI;

	Ref<EndGameResultsUI> m_EndGameHUD;

	size_t m_GoldsNb = 0;
	size_t m_PickedUpGolds = 0;

	size_t m_RunnersNb = 0;
	size_t m_DeadRunnersNb = 0;

public: // Public replicated functions

	/* Game end */

	CREATE_REPLICATED_FUNCTION
	(
		endGame_Multicast,
		[this](const PawnType& winners)
		{
			m_EndGameHUD->show(winners);
		},
		"GameState", Network::ReplicationMode::Multicast,
		const PawnType&
	);

private: // Private replicated functions

	/* Game end */

	CREATE_REPLICATED_FUNCTION
	(
		onAllGoldsPicked_Multicast,
		[this]() { this->onAllGoldsPicked(); },
		"GameState", Network::ReplicationMode::Multicast
	);

	CREATE_REPLICATED_FUNCTION
	(
		backToLobby_Multicast,
		[this]()
		{
			Application::get()->killState();
		},
	"GameState", Network::ReplicationMode::Multicast
	);
};

