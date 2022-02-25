#pragma once

#include "../Core/Base.h"

class Widget
{
public:
	Widget();
	virtual void update(const float& dt) {}
	virtual void render(Ref<sf::RenderWindow> window) {}

	inline void setParent(Ref<Widget> parent) { m_Parent = parent; }
	inline void addChild(Ref<Widget> child) { m_Children.push_back(child); }

	const sf::Vector2f& getGlobalPosition() const;
	inline const sf::Vector2f& getRelativePosition() const { return m_RelativePosition; }

protected:
	inline void renderChildren(Ref<sf::RenderWindow> window) // For more readable code
	{
		for (auto& child : m_Children)
			child->render(window);
	}

private:
	Ref<Widget> m_Parent = nullptr;
	std::vector<Ref<Widget>> m_Children;

	sf::Vector2f m_RelativePosition;
};

