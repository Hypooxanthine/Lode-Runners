#pragma once

#include "State.h"

#include "../HUD/Widgets.h"

class MainMenuState : public State
{
public:
	MainMenuState();

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private:
	Ref<Widget> m_HUD;
	Ref<ButtonWidget> m_PlayButton;
	Ref<TextWidget> m_PlayText;
	Ref<ButtonWidget> m_EditorButton;
	Ref<TextWidget> m_EditorText;
	Ref<ButtonWidget> m_SettingsButton;
	Ref<TextWidget> m_SettingsText;
};

