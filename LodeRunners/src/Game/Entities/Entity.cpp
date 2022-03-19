#include "Entity.h"

#include "../../Assets/Assets.h"
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

sf::Vector2f Entity::getPosition() const
{
	return m_WorldPosition / SPACE_UNIT;
}

void Entity::setPosition(const sf::Vector2f& position)
{
	m_WorldPosition = position * SPACE_UNIT;
}

void Entity::move(const sf::Vector2f& delta)
{
	m_WorldPosition += delta * SPACE_UNIT;
}

std::optional<Component*> Entity::getComponent(const std::string& name)
{
	for (auto& comp : m_Components)
		if (comp->getName() == name) return comp.get();

	return std::optional<Component*>();
}

void Entity::onBeginOverlap(Entity* other)
{}

void Entity::onEndOverlap(Entity* other)
{}

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
