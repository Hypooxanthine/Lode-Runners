#include "PreLobbyState.h"

#include "../Network/Network.h"
#include "LobbyState.h"

PreLobbyState::PreLobbyState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	m_TitleText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_TitleText.get(), m_HUD.get());
	m_TitleText->setGlobalPosition({ .3f, .05f });
	m_TitleText->setGlobalSize({ .4f, .1f });
	m_TitleText->setBold();
	m_TitleText->setText("Game creation");

	m_CreateServerButton = MakeRef<TextButtonWidget>("Create server");
	Widget::bindWidgets(m_CreateServerButton.get(), m_HUD.get());
	m_CreateServerButton->setGlobalPosition({ .1f, .3f });
	m_CreateServerButton->setGlobalSize({ .3f, .1f });
	m_CreateServerButton->bindCallback(BIND_FN(createServer));

	m_CreateServerPortText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_CreateServerPortText.get(), m_HUD.get());
	m_CreateServerPortText->setGlobalPosition({ .45f, .3f });
	m_CreateServerPortText->setGlobalSize({ .05f, .04f });
	m_CreateServerPortText->setText("Port");

	m_CreateServerPortTextBox = MakeRef<TextBoxWidget>();
	Widget::bindWidgets(m_CreateServerPortTextBox.get(), m_HUD.get());
	m_CreateServerPortTextBox->setGlobalPosition({ .45f, .36f });
	m_CreateServerPortTextBox->setGlobalSize({ .05f, .04f });
	m_CreateServerPortTextBox->setText("80");

	m_CreateServerMaxClientsText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_CreateServerMaxClientsText.get(), m_HUD.get());
	m_CreateServerMaxClientsText->setGlobalPosition({ .55f, .3f });
	m_CreateServerMaxClientsText->setGlobalSize({ .05f, .04f });
	m_CreateServerMaxClientsText->setText("Max clients");

	m_CreateServerMaxClientsTextBox = MakeRef<TextBoxWidget>();
	Widget::bindWidgets(m_CreateServerMaxClientsTextBox.get(), m_HUD.get());
	m_CreateServerMaxClientsTextBox->setGlobalPosition({ .55f, .36f });
	m_CreateServerMaxClientsTextBox->setGlobalSize({ .05f, .04f });
	m_CreateServerMaxClientsTextBox->setText("1");

	m_JoinServerButton = MakeRef<TextButtonWidget>("Join server");
	Widget::bindWidgets(m_JoinServerButton.get(), m_HUD.get());
	m_JoinServerButton->setGlobalPosition({ .1f, .45f });
	m_JoinServerButton->setGlobalSize({ .3f, .1f });
	m_JoinServerButton->bindCallback(BIND_FN(joinServer));

	m_JoinServerAddressText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_JoinServerAddressText.get(), m_HUD.get());
	m_JoinServerAddressText->setGlobalPosition({ .45f, .45f });
	m_JoinServerAddressText->setGlobalSize({ .1f, .04f });
	m_JoinServerAddressText->setText("Address");

	m_JoinServerAddressTextBox = MakeRef<TextBoxWidget>();
	Widget::bindWidgets(m_JoinServerAddressTextBox.get(), m_HUD.get());
	m_JoinServerAddressTextBox->setGlobalPosition({ .45f, .51f });
	m_JoinServerAddressTextBox->setGlobalSize({ .1f, .04f });
	m_JoinServerAddressTextBox->setText("localhost");

	m_JoinServerPortText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_JoinServerPortText.get(), m_HUD.get());
	m_JoinServerPortText->setGlobalPosition({ .6f, .45f });
	m_JoinServerPortText->setGlobalSize({ .05f, .04f });
	m_JoinServerPortText->setText("Port");

	m_JoinServerPortTextBox = MakeRef<TextBoxWidget>();
	Widget::bindWidgets(m_JoinServerPortTextBox.get(), m_HUD.get());
	m_JoinServerPortTextBox->setGlobalPosition({ .6f, .51f });
	m_JoinServerPortTextBox->setGlobalSize({ .05f, .04f });
	m_JoinServerPortTextBox->setText("80");

	m_PlayerNameTextBox = MakeRef<TextBoxWidget>();
	Widget::bindWidgets(m_PlayerNameTextBox.get(), m_HUD.get());
	m_PlayerNameTextBox->setGlobalPosition({ .3f, .7f });
	m_PlayerNameTextBox->setGlobalSize({ .4f, .1f });
	m_PlayerNameTextBox->setText("Player Name");

}

PreLobbyState::~PreLobbyState()
{
}

void PreLobbyState::update(const float& dt)
{
	m_HUD->update(dt);

	if (Application::get()->getEvent(EventType::Escape))
	{
		kill();
	}
}

void PreLobbyState::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);
}

void PreLobbyState::onResize()
{
	m_HUD->onResize();
}

void PreLobbyState::createServer() const
{
	// TODO : server creation and switching to lobby state
	const uint32_t port = std::stoi(m_CreateServerPortTextBox->getText());
	const size_t maxClients = std::max(0, std::stoi(m_CreateServerMaxClientsTextBox->getText()));

	if (Network::Networker::get()->createServer(maxClients, port))
	{
		pushState(MakeRef<LobbyState>(Network::Networker::get()->getPlayerID(), m_PlayerNameTextBox->getText()));
	}
}

void PreLobbyState::joinServer() const
{
	// TODO : server joining and switching to lobby state
	const uint32_t port = std::stoi(m_JoinServerPortTextBox->getText());

	if (Network::Networker::get()->createClient(m_JoinServerAddressTextBox->getText(), port))
	{
		pushState(MakeRef<LobbyState>(Network::Networker::get()->getPlayerID(), m_PlayerNameTextBox->getText()));
	}
}
