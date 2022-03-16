#pragma once

#include "State.h"

#include "../Network/Network.h"
#include "../HUD/Widgets.h"
#include "../HUD/Custom/LevelSelector.h"
#include "../HUD/Custom/TeamDispatcherUI.h"

class LobbyState : public State
{
public:
	LobbyState(const std::string& name);
	virtual ~LobbyState();

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private: // Private methods
	void onPlayerLogin(const size_t& id, const std::string& playerName);
	void onPlayerLogout(const size_t& id);
	void onServerConnexionLost();

	void launch(Ref<LevelAsset> level);

private: // Private members

	/* User interface */

	Ref<Widget> m_HUD;
	Ref<TextWidget> m_TitleText;

	Ref<TeamDispatcherUI> m_TeamDispatcher;
	Ref<LevelSelector> m_LevelSelector;
	Ref<TextButtonWidget> m_LaunchButton;

	/* Local player */

	std::string m_PlayerName;

private: // Replicated functions

	/* Logging in and out */

	CREATE_REPLICATED_FUNCTION
	(
		trigerOnPlayerLoginForAll_Multicast,
		[this](const size_t& id, const std::string& name)
		{
			onPlayerLogin(id, name);
		},
		"LobbyState", Network::ReplicationMode::Multicast, const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		trigerOnPlayerLoginForAll_OnServer, 
		[this](const size_t& id, const std::string& name) 
		{
			trigerOnPlayerLoginForAll_Multicast(id, name);
		},
		"LobbyState", Network::ReplicationMode::OnServer, const size_t&, const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		triggerOnPlayerLogoutForAll_Multicast,
		[this](const size_t& id)
		{
			onPlayerLogout(id);
		},
		"LobbyState", Network::ReplicationMode::Multicast, const size_t&
	);

	/* Level selecting */

	CREATE_REPLICATED_FUNCTION
	(
		sendLevelToAll_Multicast,
		[this](const LevelAsset& level)
		{
			this->launch(MakeRef<LevelAsset>(level));
		},
		"LobbyState", Network::ReplicationMode::Multicast, const LevelAsset&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setTeamSelectorMaxEnnemiesForAll_Multicast,
		[this](const size_t& maxEnnemies)
		{
			m_TeamDispatcher->setMaxEnnemies(maxEnnemies);
		},
		"LobbyState", Network::ReplicationMode::Multicast,
		const size_t&
	);

};

