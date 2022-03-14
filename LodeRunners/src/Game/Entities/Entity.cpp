#include "Entity.h"

#include "../Components/Component.h"

void Entity::updateComponents(const float& dt)
{
	for (Ref<Component> c : m_Components)
		c->update(dt);
}

void Entity::renderComponents(Ref<sf::RenderWindow> window)
{
	for (Ref<Component> c : m_Components)
		c->render(window);
}

std::optional<Component*> Entity::getComponent(const std::string& name)
{
	for (auto& comp : m_Components)
		if (comp->getName() == name) return comp.get();

	return std::optional<Component*>();
}

bool Entity::removeComponent(const std::string& name)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->getName() == name)
		{
			m_Components.erase(m_Components.begin() + i);
			return true;
		}
	}

	return false;
}
