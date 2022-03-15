#pragma once
#include "../../Core/Base.h"

class State;
class Component;

class Entity
{
public:
	Entity() = default;

	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}
	void updateComponents(const float& dt);
	void renderComponents(Ref<sf::RenderWindow> window);

	const sf::Vector2f& getPosition() const { return m_WorldPosition; }
	void setPosition(const sf::Vector2f& position) { m_WorldPosition = position; }
	void move(const sf::Vector2f& delta) { m_WorldPosition += delta; }

	std::optional<Component*> getComponent(const std::string& name);

public: // Events
	virtual void onBeginOverlap(Entity* other) {}
	virtual void onEndOverlap(Entity* other) {}

protected: // Protected methods (for children)
	// A component can only be added to an Entity via this method.
	template <typename C, typename... Args>
	C* makeComponent(const std::string& name, Args&&... args)
	{
		ASSERT((std::is_base_of<Component, C>::value), "C should inherit from Component.");

		Ref<C> c = MakeRef<C>(std::forward<Args>(args)...);
		c->setName(name);
		c->setParent(this);
		m_Components.push_back(c);

		return dynamic_cast<C*>(m_Components.back().get());
	}

	bool removeComponent(const std::string& name);

private: // Private attributes
	sf::Vector2f m_WorldPosition;
	std::vector<Ref<Component>> m_Components;
};

