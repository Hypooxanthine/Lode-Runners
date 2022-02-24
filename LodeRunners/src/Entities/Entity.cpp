#include "Entity.h"
#include "../Components/FlipbookComponent.h"

Entity::Entity()
{
	auto test = makeComponent<FlipbookComponent>();
}

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

template <typename C, typename... Args>
Ref<C> Entity::makeComponent(Args&&... args)
{
	ASSERT((std::is_base_of<Component, C>::value), "C should inherit from Component.");

	auto c = MakeRef<C>(std::forward<Args>(args)...);
	m_Components.push_back(c);

	return c;
}