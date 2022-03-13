#pragma once

#include "State.h"
#include "../HUD/Widgets.h"

class PreMenuState : public State
{
public:
	PreMenuState();

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void onResize() override;

private: // Private methods

private: // Private members
	Ref<Widget> m_HUD;
	Ref<TextWidget> m_TitleText;
	Ref<TextWidget> m_BodyText;
	Ref<ButtonWidget> m_LaunchButton;
	Ref<TextWidget> m_LaunchText;

	sf::RectangleShape m_AnimShape;
	float m_AnimEllapsedTime = 0.f;

	sf::View m_View;
};

