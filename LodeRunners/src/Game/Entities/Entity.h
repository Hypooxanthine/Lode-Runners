#pragma once

#include "../../Core/Base.h"

// Tile size can differs depending on the skin. So the space unit is the tile size.
// (0.f, 0.f) is top-left corner, (26.f, 16.f) is bottom_right corner.
#define SPACE_UNIT (float)Assets::getElementSize()

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

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	void move(const sf::Vector2f& delta);

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

