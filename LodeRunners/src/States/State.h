#pragma once

#include "../Core/Base.h"
#include "../Core/Application.h"

class State
{
public:
	State() = default;

	// To be overriden be children (it has to return the next state to be pushed ; could be nullptr)
	virtual inline Ref<State> getNextState() = 0;

	virtual void init() {}
	virtual void update(const float& dt) {}
	virtual void render(const Ref<sf::RenderWindow>& window) { window->clear(); }

protected:
	inline void kill() { Application::get()->killState( getNextState() ); }
};

