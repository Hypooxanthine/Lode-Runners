#pragma once
#include "../Core/Base.h"
#include "../Components/Component.h"

class Entity
{
public:
	Entity();

	virtual void update(const float& dt);
	virtual void render(Ref<sf::RenderWindow> window);

protected:
	template <typename C, typename... Args>
	Ref<C> makeComponent(Args&&... args);

private:
	sf::Vector2f m_WorldPosition;
	std::vector<Ref<Component>> m_Components;
};

