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
	virtual const sf::Color& getClearColor() const;

protected:
	inline void kill() { Application::get()->killState(); }
	inline void pushState(const Ref<State> state) const { Application::get()->pushState(state); }
};

