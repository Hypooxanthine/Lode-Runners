#pragma once

#include "../../Core/Base.h"

#include "../Entities/Entity.h"

class Component
{
public:
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}

	void setParent(Entity* p) { m_Parent = p; }

	const std::string& getName() const { return m_Name; }
	void setName(const std::string& name) { m_Name = name; }

	const sf::Vector2f& getRelativePosition() const { return m_RelativePosition; }
	sf::Vector2f getWorldPosition() const;

	void setRelativePosition(const sf::Vector2f& pos) { m_RelativePosition = pos; }

protected:
	Entity* getParent() { return m_Parent; }
	const Entity* getParent() const { return m_Parent; }

private:
	std::string m_Name;
	Entity* m_Parent = nullptr;

	sf::Vector2f m_RelativePosition;
};

