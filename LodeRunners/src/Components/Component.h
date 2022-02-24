#pragma once

#include "../Core/Base.h"

class Entity;

class Component
{
public:
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}

	void setParent(Entity* p) { m_Parent = p; }

private:
	Entity* m_Parent = nullptr;
};

