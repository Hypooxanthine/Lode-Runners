#pragma once

#include "State.h"

#include "../Network/Network.h"
#include "../HUD/Widgets.h"

class LobbyState : public State
{
public:
	LobbyState(const std::string& name);

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

	void onPlayerLogin(const std::string& playerName);
	void onPlayerLogout();

private: // Private methods

private: // Private members
	Ref<Widget> m_HUD;
	std::string m_LocalName;

	std::vector<std::string> m_PlayerNames;
	sf::Vector2f m_NextTextWidgetPos = { .1f, .1f };

private: // Replicated functions
	CREATE_REPLICATED_FUNCTION
	(
		onPlayerLoginClients,
		[this](const std::string& name)
		{
			this->onPlayerLogin(name);
		},
		"", const std::string&
	);

	CREATE_REPLICATED_FUNCTION
	(
		onPlayerLoginServer, 
		[this](const std::string& name) 
		{
			onPlayerLogin(name);

			for(auto& pn: m_PlayerNames)
				GET_REPLICATED_FUNCTION(onPlayerLoginClients).call(Network::ReplicationMode::OnClients, pn);
		},
		"", const std::string&
	);


};

