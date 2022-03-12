#pragma once

#include "State.h"

#include "../Network/Network.h"
#include "../HUD/Widgets.h"

class LobbyState : public State
{
public:
	LobbyState(const size_t& id, const std::string& name);

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private: // Private methods
	void getLoggedPlayers();
	void onPlayerLogin(const size_t& id, const std::string& playerName);
	void onPlayerLogout(const size_t& id);

	void createPlayerTextWidget(const size_t& id, const std::string& name);

private: // Private members
	Ref<Widget> m_HUD;
	std::vector<Ref<TextWidget>> m_TextWidgets;

	size_t m_PlayerID;
	std::string m_PlayerName;

	std::vector<std::pair<size_t, std::string>> m_Players;
	sf::Vector2f m_NextTextWidgetPos = { .1f, .1f };

private: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		sendLoggedPlayersFromAsker_Multicast,
		[this](const size_t& asker, const size_t& playerID, const std::string& playerName)
		{
			if (asker == m_PlayerID)
			{
				m_Players.emplace_back(playerID, playerName);
				createPlayerTextWidget(playerID, playerName);
			}
		},
		"LobbyState", Network::ReplicationMode::Multicast, const size_t&, const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		getLoggedPlayersFromAsker_OnServer,
		[this](const size_t& asker)
		{
			for(auto& p : m_Players)
				sendLoggedPlayersFromAsker_Multicast(asker, p.first, p.second);
		},
		"LobbyState", Network::ReplicationMode::OnServer, const size_t&
	);

	CREATE_REPLICATED_FUNCTION
	(
		registerPlayerForAll_Multicast,
		[this](const size_t& id, const std::string& name)
		{
			onPlayerLogin(id, name);
		},
		"LobbyState", Network::ReplicationMode::Multicast, const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		registerPlayerForAll_OnServer, 
		[this](const size_t& id, const std::string& name) 
		{
			registerPlayerForAll_Multicast(id, name);
		},
		"LobbyState", Network::ReplicationMode::OnServer, const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		removePlayerForAll_Multicast,
		[this](const size_t& id)
		{
			onPlayerLogout(id);
		},
		"LobbyState", Network::ReplicationMode::Multicast, const size_t&
	);

	CREATE_REPLICATED_FUNCTION
	(
		removePlayerForAll_OnServer,
		[this](const size_t& id)
		{
			removePlayerForAll_Multicast(id);
		},
		"LobbyState", Network::ReplicationMode::OnServer, const size_t&
	);

};

