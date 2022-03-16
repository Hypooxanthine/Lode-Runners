#pragma once

#include "../Widgets.h"

#include "../../Network/Network.h"

class TeamDispatcherUI : public Widget
{
public:
	TeamDispatcherUI(const size_t& maxEnnemies, const std::string& playerName);
	TeamDispatcherUI(Widget* parent, const size_t& maxEnnemies, const std::string& playerName);

	void setMaxEnnemies(const size_t& val);

private: // Private member functions
	void setPlayerToRunner(const size_t& playerID, const std::string& playerName);
	void setPlayerToEnnemy(const size_t& playerID, const std::string& playerName);

	void updateNumbersTexts();

	std::tuple<size_t, std::string, Ref<TextWidget>> makeRunnerEntry(const size_t& playerID, const std::string& playerName);
	std::tuple<size_t, std::string, Ref<TextWidget>> makeEnnemyEntry(const size_t& playerID, const std::string& playerName);

	bool isEnnemiesListFull() const { return m_Ennemies.size() == m_MaxEnnemies; }
	bool canRemovePlayerFromRunners() const { return m_Runners.size() > 1; }
	bool playerBelongsToRunners(const size_t& playerID) const;
	bool playerBelongsToEnnemies(const size_t& playerID) const;

private: // Private members
	std::string m_Name;

	Ref<TextWidget> m_RunnersTitleText, m_EnnemiesTitleText;
	Ref<TextWidget> m_RunnersNumberText, m_EnnemiesNumberText;

	Ref<Widget> m_RunnersField, m_EnnemiesField;
	std::vector<std::tuple<size_t, std::string, Ref<TextWidget>>> m_Runners, m_Ennemies;

	Ref<TextButtonWidget> m_JoinRunnersButton, m_JoinEnnemiesButton;

	size_t m_MaxEnnemies = 0;

private: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		sendInitDataFromAsker_Multicast,
		[this](const size_t& asker, const size_t& playerID, const std::string& playerName, const size_t& maxEnnemies, const bool& runner)
		{
			if (asker == PLAYER_ID)
			{
				if (runner)
					this->setPlayerToRunner(playerID, playerName);
				else
					this->setPlayerToEnnemy(playerID, playerName);

				this->setMaxEnnemies(maxEnnemies);
			}
		},
		"TeamDispatcherUI", Network::ReplicationMode::Multicast,
		const size_t&, const size_t&, const std::string&, const size_t&, const bool&
	);

	CREATE_REPLICATED_FUNCTION
	(
		getInitDataFromAsker_OnServer,
		[this](const size_t& asker)
		{
			for (auto& r : this->m_Runners)
				sendInitDataFromAsker_Multicast(asker, std::get<0>(r), std::get<1>(r), m_MaxEnnemies, true);
			for(auto& e : this->m_Ennemies)
				sendInitDataFromAsker_Multicast(asker, std::get<0>(e), std::get<1>(e), m_MaxEnnemies, false);
		},
		"TeamDispatcherUI", Network::ReplicationMode::OnServer,
		const size_t&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setPlayerToRunnerForAll_Multicast,
		[this](const size_t& playerID, const std::string& playerName) -> void
		{
			LOG_INFO("Player " + playerName + " joining runners.");
			this->setPlayerToRunner(playerID, playerName);
		},
		"TeamDispatcherUI", Network::ReplicationMode::Multicast,
		const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setPlayerToEnnemyForAll_Multicast,
		[this](const size_t& playerID, const std::string& playerName) -> void
		{
			LOG_INFO("Player " + playerName + " joining ennemies.");
			this->setPlayerToEnnemy(playerID, playerName);
		},
		"TeamDispatcherUI", Network::ReplicationMode::Multicast,
		const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		requestJoinRunners_Server,
		[this](const size_t& playerID, const std::string& playerName) -> void
		{
			if(!playerBelongsToRunners(playerID))
				this->setPlayerToRunnerForAll_Multicast(playerID, playerName);
		},
		"TeamDispatcherUI", Network::ReplicationMode::OnServer,
		const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		requestJoinEnnemies_Server,
		[this](const size_t& playerID, const std::string& playerName) -> void
		{
			// If the player belongs to no list (just arrived to the lobby), or if he belongs to runners AND
			// runners list has at least 2 players, then server accepts to set player's team to ennemies.
			// Runners list has to contain at least 1 player.
			if (!playerBelongsToEnnemies(playerID) && canRemovePlayerFromRunners())
				this->setPlayerToEnnemyForAll_Multicast(playerID, playerName);
		},
		"TeamDispatcherUI", Network::ReplicationMode::OnServer,
		const size_t&, const std::string&
	);

};

