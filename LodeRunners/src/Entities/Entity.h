#pragma once
#include "../Core/Base.h"
#include "../Components/Component.h"

class State;

class Entity
{
public:
	Entity();

	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}
	void updateComponents(const float& dt);
	void renderComponents(Ref<sf::RenderWindow> window);

protected: // Protected methods (for children)
	template <typename C, typename... Args>
	Ref<C> makeComponent(Args&&... args);

private: // Private attributes
	sf::Vector2f m_WorldPosition;
	std::vector<Ref<Component>> m_Components;
};

