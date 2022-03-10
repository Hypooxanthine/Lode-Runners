#include "PreLobbyState.h"

#include "../Network/Network.h"

PreLobbyState::PreLobbyState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	auto titleText = MakeRef<TextWidget>();
	Widget::addChild(titleText, m_HUD);
	titleText->setGlobalPosition({ .3f, .05f });
	titleText->setGlobalSize({ .4f, .1f });
	titleText->setBold();
	titleText->setText("Game creation");

	auto createServerButton = MakeRef<ButtonWidget>();
	Widget::addChild(createServerButton, m_HUD);
	createServerButton->setGlobalPosition({ .1f, .3f });
	createServerButton->setGlobalSize({ .3f, .1f });
	createServerButton->bindCallback(BIND_FN(createServer));

	auto createServerText = MakeRef<TextWidget>();
	Widget::addChild(createServerText, createServerButton);
	createServerText->fillParent();
	createServerText->setText("Create Server");

	auto createServerPortText = MakeRef<TextWidget>();
	Widget::addChild(createServerPortText, m_HUD);
	createServerPortText->setGlobalPosition({ .45f, .3f });
	createServerPortText->setGlobalSize({ .05f, .04f });
	createServerPortText->setText("Port");

	m_CreateServerPortTextBox = MakeRef<TextBoxWidget>();
	Widget::addChild(m_CreateServerPortTextBox, m_HUD);
	m_CreateServerPortTextBox->setGlobalPosition({ .45f, .36f });
	m_CreateServerPortTextBox->setGlobalSize({ .05f, .04f });
	m_CreateServerPortTextBox->setText("80");

	auto createServerMaxClientsText = MakeRef<TextWidget>();
	Widget::addChild(createServerMaxClientsText, m_HUD);
	createServerMaxClientsText->setGlobalPosition({ .55f, .3f });
	createServerMaxClientsText->setGlobalSize({ .05f, .04f });
	createServerMaxClientsText->setText("Max clients");

	m_CreateServerMaxClientsTextBox = MakeRef<TextBoxWidget>();
	Widget::addChild(m_CreateServerMaxClientsTextBox, m_HUD);
	m_CreateServerMaxClientsTextBox->setGlobalPosition({ .55f, .36f });
	m_CreateServerMaxClientsTextBox->setGlobalSize({ .05f, .04f });
	m_CreateServerMaxClientsTextBox->setText("1");

	auto joinServerButton = MakeRef<ButtonWidget>();
	Widget::addChild(joinServerButton, m_HUD);
	joinServerButton->setGlobalPosition({ .1f, .45f });
	joinServerButton->setGlobalSize({ .3f, .1f });
	joinServerButton->bindCallback(BIND_FN(joinServer));

	auto joinServerText = MakeRef<TextWidget>();
	Widget::addChild(joinServerText, joinServerButton);
	joinServerText->fillParent();
	joinServerText->setText("Join Server");

	auto joinServerAddressText = MakeRef<TextWidget>();
	Widget::addChild(joinServerAddressText, m_HUD);
	joinServerAddressText->setGlobalPosition({ .45f, .45f });
	joinServerAddressText->setGlobalSize({ .1f, .04f });
	joinServerAddressText->setText("Address");

	m_JoinServerAddressTextBox = MakeRef<TextBoxWidget>();
	Widget::addChild(m_JoinServerAddressTextBox, m_HUD);
	m_JoinServerAddressTextBox->setGlobalPosition({ .45f, .51f });
	m_JoinServerAddressTextBox->setGlobalSize({ .1f, .04f });
	m_JoinServerAddressTextBox->setText("localhost");

	auto joinServerPortText = MakeRef<TextWidget>();
	Widget::addChild(joinServerPortText, m_HUD);
	joinServerPortText->setGlobalPosition({ .6f, .45f });
	joinServerPortText->setGlobalSize({ .05f, .04f });
	joinServerPortText->setText("Port");

	m_JoinServerPortTextBox = MakeRef<TextBoxWidget>();
	Widget::addChild(m_JoinServerPortTextBox, m_HUD);
	m_JoinServerPortTextBox->setGlobalPosition({ .6f, .51f });
	m_JoinServerPortTextBox->setGlobalSize({ .05f, .04f });
	m_JoinServerPortTextBox->setText("80");

}

void PreLobbyState::init()
{
	onResize();
}

void PreLobbyState::update(const float& dt)
{
	m_HUD->update(dt);

	if (Application::get()->getEvent(EventType::Escape))
	{
		Network::Networker::get()->reset();
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
	const size_t maxClients = std::abs(std::stoi(m_CreateServerMaxClientsTextBox->getText()));

	if (Network::Networker::get()->createServer(maxClients, port))
	{

	}
}

void PreLobbyState::joinServer() const
{
	// TODO : server joining and switching to lobby state
	const uint32_t port = std::stoi(m_JoinServerPortTextBox->getText());

	if (Network::Networker::get()->createClient(m_JoinServerAddressTextBox->getText(), port))
	{

	}
}
