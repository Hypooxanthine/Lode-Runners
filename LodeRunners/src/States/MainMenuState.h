#pragma once

#include "State.h"

#include "../HUD/Widgets.h"

class MainMenuState : public State
{
public:
	MainMenuState();

	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private:
	Ref<Widget> m_HUD;
	Ref<TextButtonWidget> m_PlayButton;
	Ref<TextButtonWidget> m_EditorButton;
	Ref<TextButtonWidget> m_SettingsButton;
};

