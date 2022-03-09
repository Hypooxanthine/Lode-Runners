#pragma once

#include "State.h"

#include "../HUD/TextWidget.h"
#include "../HUD/ButtonWidget.h"
#include "../HUD/TextBoxWidget.h"

class PreLobbyState : public State
{
public:
	PreLobbyState();

	virtual void init() override;
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
};

