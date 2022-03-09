#pragma once

#include "State.h"
#include "../HUD/Widget.h"

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

	sf::RectangleShape m_AnimShape;
	float m_AnimEllapsedTime = 0.f;

	sf::View m_View;
};

