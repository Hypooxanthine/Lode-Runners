#pragma once

#include "../Core/Base.h"

class State
{
public:
	State() = default;

	virtual void init() {}
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow>& window) {}

	// To be overriden by children
	virtual void onResize(const sf::Vector2u& size) = 0;

protected:
	inline void kill() { Application::get()->killState(); }
	inline void pushState(const Ref<State> state) { Application::get()->pushState(state); }
};

