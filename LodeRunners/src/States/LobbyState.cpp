#include "LobbyState.h"

LobbyState::LobbyState(const size_t& id, const std::string& name)
	: State(), m_PlayerID(id), m_PlayerName(name)
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	if (IS_SERVER)
		Network::Networker::get()->bindOnPlayerLogout([this](const size_t& id) { removePlayerForAll_OnServer(id); });
}

void LobbyState::init()
{
	getLoggedPlayers();
	registerPlayerForAll_OnServer(m_PlayerID, m_PlayerName);
}

void LobbyState::update(const float& dt)
{
	m_HUD->update(dt);
}

void LobbyState::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);
}

void LobbyState::onResize()
{
	m_HUD->onResize();
}

void LobbyState::getLoggedPlayers()
{
	getLoggedPlayersFromAsker_OnServer(m_PlayerID);
}

void LobbyState::onPlayerLogin(const size_t& id, const std::string& playerName)
{
	LOG_INFO("Player login. Name : " + playerName + ", ID : " + std::to_string(id) + ".");
	m_Players.emplace_back(id, playerName);
	createPlayerTextWidget(id, playerName);
}

void LobbyState::onPlayerLogout(const size_t& id)
{
	bool found = false;

	// Removing player with playerID 'id' in m_Players.
	for (size_t i = m_Players.size(); i > 0 && !found; i--)
	{
		const auto& p = m_Players[i - 1];
		if (p.first == id)
		{
			m_Players.erase(m_Players.begin() + i - 1);
			found = true;
		}
	}

	if (!found) return;

	// Removing TextWidgets.
	m_HUD->removeChildren();
	//m_TextWidgets.clear();
	m_NextTextWidgetPos.x = .1f;
	m_NextTextWidgetPos.y = .1f;

	// Recreating TextWidgets.
	for (auto& p : m_Players)
		createPlayerTextWidget(p.first, p.second);
}

void LobbyState::createPlayerTextWidget(const size_t& id, const std::string& name)
{
	auto textWidget = MakeRef<TextWidget>();
	Widget::addChild(textWidget, m_HUD);
	textWidget->setGlobalPosition(m_NextTextWidgetPos);
	textWidget->setGlobalSize({ .1f, .05f });
	textWidget->setCenterX(false);
	textWidget->setText(name + " (" + std::to_string(id) + ")");
	//m_TextWidgets.push_back(textWidget);

	m_NextTextWidgetPos += {0.f, .05f};
}
