#pragma once

#include "../Core/Base.h"

class Component
{
public:
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}
};

