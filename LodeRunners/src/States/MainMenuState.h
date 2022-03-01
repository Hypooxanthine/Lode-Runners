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
	virtual inline const sf::Color& getClearColor() const override
	{
		static auto c = sf::Color::Color(10, 10, 10);
		return c;
	}

private:
	Ref<Widget> m_HUD;

};

