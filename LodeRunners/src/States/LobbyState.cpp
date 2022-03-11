#include "LobbyState.h"

LobbyState::LobbyState(const std::string& name)
	: State(), m_LocalName(name)
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();
}

void LobbyState::init()
{
	GET_REPLICATED_FUNCTION(onPlayerLoginServer).call(Network::ReplicationMode::OnServer, m_LocalName);
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

void LobbyState::onPlayerLogin(const std::string& playerName)
{
	LOG_INFO("Player connected : " + playerName);
	m_PlayerNames.push_back(playerName);

	auto textWidget = MakeRef<TextWidget>();
	Widget::addChild(textWidget, m_HUD);
	textWidget->setGlobalPosition(m_NextTextWidgetPos);
	textWidget->setGlobalSize({ .1f, .05f });
	textWidget->setCenterX(false);
	textWidget->setText(playerName);
	
	m_NextTextWidgetPos += {0.f, .05f};
}

void LobbyState::onPlayerLogout()
{
}
