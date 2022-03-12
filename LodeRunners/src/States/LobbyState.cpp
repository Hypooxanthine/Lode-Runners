#include "LobbyState.h"

LobbyState::LobbyState(const size_t& id, const std::string& name)
	: State(), m_PlayerID(id), m_PlayerName(name)
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	m_PlayersTexts = MakeRef<Widget>();
	Widget::addChild(m_PlayersTexts, m_HUD);
	m_PlayersTexts->setRelativePosition({ 0.f, .1f });
	m_PlayersTexts->setRelativeSize({ 1.f, .9f });

	auto titleText = MakeRef<TextWidget>();
	Widget::addChild(titleText, m_HUD);
	titleText->setRelativePosition({ .3f, 0.f });
	titleText->setRelativeSize({ .4f, .1f });
	titleText->setBold();
	titleText->setText("Lobby");

	if (IS_SERVER)
		Network::Networker::get()->bindOnPlayerLogout([this](const size_t& id) { triggerOnPlayerLogoutForAll_OnServer(id); });
	else
		Network::Networker::get()->bindOnServerConnexionLost([this]() { kill(); });
}

LobbyState::~LobbyState()
{
	if (IS_SERVER)
		Network::Networker::get()->popOnPlayerLogout();
	else
		Network::Networker::get()->popOnServerConnexionLost();

	Network::Networker::get()->reset();
}

void LobbyState::init()
{
	getLoggedPlayersFromAsker_OnServer(m_PlayerID);
	trigerOnPlayerLoginForAll_OnServer(m_PlayerID, m_PlayerName);
}

void LobbyState::update(const float& dt)
{
	if (Application::get()->getEvent(EventType::Escape))
		kill();

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
	m_PlayersTexts->removeChildren();
	m_NextTextWidgetPos.x = 0.f;
	m_NextTextWidgetPos.y = 0.f;

	// Recreating TextWidgets.
	for (auto& p : m_Players)
		createPlayerTextWidget(p.first, p.second);
}

void LobbyState::onServerConnexionLost()
{
}

void LobbyState::createPlayerTextWidget(const size_t& id, const std::string& name)
{
	auto textWidget = MakeRef<TextWidget>();
	Widget::addChild(textWidget, m_PlayersTexts);
	textWidget->setRelativePosition(m_NextTextWidgetPos);
	textWidget->setRelativeSize({ .1f, .05f });
	textWidget->setCenterX(false);
	textWidget->setText(name + " (" + std::to_string(id) + ")");
	if (id == m_PlayerID)
	{
		textWidget->setColor(sf::Color::Red);
		textWidget->setBold();
	}

	m_NextTextWidgetPos += {0.f, .05f};
}
