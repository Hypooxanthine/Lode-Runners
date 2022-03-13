#include "LobbyState.h"

LobbyState::LobbyState(const size_t& id, const std::string& name)
	: State(), m_PlayerID(id), m_PlayerName(name)
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	m_PlayersTextsAnchor = MakeRef<Widget>();
	Widget::bindWidgets(m_PlayersTextsAnchor.get(), m_HUD.get());
	m_PlayersTextsAnchor->setRelativePosition({ 0.f, .1f });
	m_PlayersTextsAnchor->setRelativeSize({ 1.f, .8f });

	m_TitleText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_TitleText.get(), m_HUD.get());
	m_TitleText->setRelativePosition({ .3f, 0.f });
	m_TitleText->setRelativeSize({ .4f, .1f });
	m_TitleText->setBold();
	m_TitleText->setText("Lobby");

	if (IS_SERVER)
	{
		m_LevelSelector = MakeRef<LevelSelector>(m_HUD.get());
		m_LevelSelector->setRelativePosition({ .1f, .85f });
		m_LevelSelector->setRelativeSize({ .6f, .1f });

		m_LaunchButton = MakeRef<ButtonWidget>(m_HUD.get());
		m_LaunchButton->setRelativePosition({ .75f, .85f });
		m_LaunchButton->setRelativeSize({ .2f, .1f });
		m_LaunchButton->bindCallback
		(
			[this]()
			{
				sendLevelToAll_Multicast(*Assets::getLevelAsset(m_LevelSelector->getSelectedLevelName()));
			}
		);

		m_LaunchText = MakeRef<TextWidget>(m_LaunchButton.get());
		m_LaunchText->fillParent();
		m_LaunchText->setText("Launch");
	}

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
	m_HUD->onResize();

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
	m_PlayersTextsAnchor->removeChildren();
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
	m_PlayersText.emplace_back(MakeRef<TextWidget>());
	Widget::bindWidgets(m_PlayersText.back().get(), m_PlayersTextsAnchor.get());
	m_PlayersText.back()->setRelativePosition(m_NextTextWidgetPos);
	m_PlayersText.back()->setRelativeSize({ .1f, .05f });
	m_PlayersText.back()->setCenterX(false);
	m_PlayersText.back()->setText(name + " (" + std::to_string(id) + ")");
	if (id == m_PlayerID)
	{
		m_PlayersText.back()->setColor(sf::Color::Red);
		m_PlayersText.back()->setBold();
	}


	m_NextTextWidgetPos += {0.f, .05f};
}

void LobbyState::launch(const Ref<LevelAsset>& level)
{
	LOG_INFO("Level received. Name : " + level->getName());

	for (size_t i = 0; i < level->getSize(); i++)
	{
		if (i % TILES_WIDTH == 0 && i != 0) std::cout << "\n";
		std::cout << (short)(level->at(i)->getType()) << " ";
	}

	std::cout << "\n";
}
