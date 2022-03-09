#pragma once

#include "../Core/Base.h"

class State
{
public:
	State() = default;

	// To be overriden by children
	virtual void init() {}
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}
	virtual void onResize() {}
	virtual inline const sf::Color& getClearColor() const
	{
		static auto c = sf::Color::Color(10, 10, 10);
		return c;
	}

protected:
	inline void kill() { Application::get()->killState(); }
	inline void pushState(const Ref<State> state) { Application::get()->pushState(state); }
};

