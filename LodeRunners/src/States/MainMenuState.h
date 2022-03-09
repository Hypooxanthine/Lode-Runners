#pragma once

#include "State.h"

#include "../HUD/Widget.h"

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

};

