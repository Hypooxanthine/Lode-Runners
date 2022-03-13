#pragma once

#include "State.h"

#include "../HUD/Widgets.h"

class PreLobbyState : public State
{
public:
	PreLobbyState();
	virtual ~PreLobbyState();

	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private: // Private methods
	void createServer() const;
	void joinServer() const;

private:
	Ref<Widget> m_HUD;
	Ref<TextBoxWidget> m_CreateServerPortTextBox;
	Ref<TextBoxWidget> m_CreateServerMaxClientsTextBox;
	Ref<TextBoxWidget> m_JoinServerAddressTextBox;
	Ref<TextBoxWidget> m_JoinServerPortTextBox;
	Ref<TextBoxWidget> m_PlayerNameTextBox;

	Ref<TextWidget> m_TitleText;

	Ref<TextButtonWidget> m_CreateServerButton;

	Ref<TextWidget> m_CreateServerPortText;
	Ref<TextWidget> m_CreateServerMaxClientsText;

	Ref<TextButtonWidget> m_JoinServerButton;

	Ref<TextWidget> m_JoinServerAddressText;
	Ref<TextWidget> m_JoinServerPortText;
};

