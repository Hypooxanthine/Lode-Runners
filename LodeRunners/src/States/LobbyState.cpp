#include "LobbyState.h"

#include "GameState.h"

LobbyState::LobbyState(const std::string& name)
	: State(), m_PlayerName(name)
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	m_TitleText = MakeRef<TextWidget>(m_HUD.get());
	m_TitleText->setRelativePosition({ .3f, 0.f });
	m_TitleText->setRelativeSize({ .4f, .1f });
	m_TitleText->setBold();
	m_TitleText->setText("Lobby");

	m_TeamDispatcher = MakeRef<TeamDispatcherUI>(m_HUD.get(), m_PlayerName);
	m_TeamDispatcher->setRelativePosition({ .1f, .15f });
	m_TeamDispatcher->setRelativeSize({ .8f, .65f });

	// UI for selecting the level to be played
	if (IS_SERVER)
	{
		m_LevelSelector = MakeRef<LevelSelector>
		(
			m_HUD.get(),
			[this](const size_t& levelIndex, const std::string& levelName)
			{
				setTeamSelectorMaxEnnemiesForAll_Multicast(Assets::getLevelAsset(levelName)->getEnnemiesStart().size());
			}
		);
		m_LevelSelector->setRelativePosition({ .1f, .85f });
		m_LevelSelector->setRelativeSize({ .6f, .1f });

		m_LaunchButton = MakeRef<TextButtonWidget>(m_HUD.get(), "Launch");
		m_LaunchButton->setRelativePosition({ .75f, .85f });
		m_LaunchButton->setRelativeSize({ .2f, .1f });
		m_LaunchButton->bindCallback
		(
			[this]()
			{
				sendLevelToAll_Multicast(*Assets::getLevelAsset(m_LevelSelector->getSelectedLevelName(), true));
			}
		);
	}

	// When the server (client side) / a client (server side) logs out
	if (IS_SERVER)
		Network::Networker::get()->bindOnPlayerLogout([this](const size_t& id) { triggerOnPlayerLogoutForAll_Multicast(id); });
	else
		Network::Networker::get()->bindOnServerConnexionLost([this]() { kill(); });

}

LobbyState::~LobbyState()
{
	Network::Networker::get()->reset();
}

void LobbyState::init()
{
	State::init();

	trigerOnPlayerLoginForAll_OnServer(PLAYER_ID, m_PlayerName);
	Network::Networker::get()->acceptClients();
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
}

void LobbyState::onPlayerLogout(const size_t& id)
{
}

void LobbyState::onServerConnexionLost()
{
}

void LobbyState::launch(Ref<LevelAsset> level)
{
	#ifdef _DEBUG
	LOG_INFO("Level received. Name : " + level->getName());

	for (size_t i = 0; i < level->getSize(); i++)
	{
		if (i % TILES_WIDTH == 0 && i != 0) std::cout << "\n";
		std::cout << (short)(level->at(i)->getType()) << " ";
	}
	std::cout << "\n";

	#endif

	if(level->isValid())
	{
		Network::Networker::get()->stopAcceptingClients();
		pushState(MakeRef<GameState>(level, m_TeamDispatcher->getRunners(), m_TeamDispatcher->getEnnemies()));
	}
	else
	{
		LOG_WARN("Couldn't launch game with level " + level->getName() + " : level not valid.");
	}
}
