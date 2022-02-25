#pragma once

#include "../Core/Base.h"

class CursorRay
{
private:
	CursorRay() = default;

	inline void consume() { consumed = true; }

	// if(CursorRay) -> condition is true if the ray has not been consumed yet.
	operator bool() const { return !consumed; }

	const sf::Vector2i& getPosition() const { return m_Position; }
	const bool& isPressed() const { return m_Pressed; }
private:
	// When a CursorRay is used (example : it hovers a button, so the button changes his color and consumes the ray), the widget that uses it has to make this value as true. No verification are done when trying to access data. A widget can see data even if his consumed member is true. He just has to know that another widget has responded to the position member value.
	bool consumed = false;
	sf::Vector2i m_Position = sf::Mouse::getPosition(*Application::get()->getWindow());
	bool m_Pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
};

class Widget
{
public:
	Widget();
	void update(const float& dt); // This function first calculates a CursorRay then internally calls an update function which takes this CursorRay as parameter.
	virtual void render(Ref<sf::RenderWindow> window) {}

	inline void setParent(Ref<Widget> parent) { m_Parent = parent; }
	inline void addChild(Ref<Widget> child) { m_Children.push_back(child); }

	const sf::Vector2f& getGlobalPosition() const;
	inline const sf::Vector2f& getRelativePosition() const { return m_RelativePosition; }

protected:
	virtual void update(const float& dt, CursorRay& ray) {}

	// We must first update children, then update this. If children have something to do with a ray, they handle it and then parent don't. If they don't handle the ray, their parent can handle it, recursively.
	// Example : a Button widget has a Text widget as child. The Text widget first receives the CursorRay. It obviously has nothing to do with a ray, so it does not use it, and its "consumed" member stays false. Then the Button widget handles the ray. If the ray is inside its bounding box, it consumes it, else, it does not consume it (eventually the ray can be used by the Button widget's parent).
	void updateChildren(const float& dt, CursorRay& ray); // For more readable code
	// We must first render this, then children. Children have to show in front of their parent.
	void renderChildren(Ref<sf::RenderWindow> window); // For more readable code

private:
	Ref<Widget> m_Parent = nullptr;
	std::vector<Ref<Widget>> m_Children;

	sf::Vector2f m_RelativePosition;
};
