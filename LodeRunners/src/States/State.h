#pragma once

#include "../Core/Base.h"

class State
{
public:
	State() = default;

	virtual void init() {}
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow>& window) {}

protected:
	inline void kill() { Application::get()->killState(); }
	inline void pushState(const Ref<State> state) { Application::get()->pushState(state); }
};

