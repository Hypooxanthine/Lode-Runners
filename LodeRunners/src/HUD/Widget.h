#pragma once

#include "../Core/Base.h"

class CursorRay
{
public:
	CursorRay() = default;

	void create(const sf::View& view)
	{
		consumed = false;
		const auto window = Application::get()->getWindow();
		const auto mousePos = sf::Mouse::getPosition(*window);
		m_Position = window->mapPixelToCoords(mousePos, view);
		m_Pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	inline void consume() { consumed = true; }

	// if(CursorRay) -> condition is true if the ray has not been consumed yet.
	operator bool() const { return !consumed; }

	const sf::Vector2f& getPosition() const { return m_Position; }
	const bool& isPressed() const { return m_Pressed; }
private:
	// When a CursorRay is used (example : it hovers a button, so the button changes his color and consumes the ray), the widget that uses it has to make this value as true. No verification are done when trying to access data. A widget can see data even if his consumed member is true. He just has to know that another widget has responded to the position member value.
	bool consumed = false;
	sf::Vector2f m_Position;
	bool m_Pressed = false;
};

class Widget
{
public:
	Widget();

	// This function first calculates a CursorRay then calls update(const float&, CursorRay&).
	void update(const float& dt);
	// This function updates children then updates "this". More explainations in implementation.
	void update(const float& dt, CursorRay& ray);
	// This function first renders "this" then renders children. More explainations in implementation.
	void render(Ref<sf::RenderWindow> window);
	void onResize();
	void onResize(const sf::Vector2f& size);

	void setParent(Ref<Widget> parent);
	void addChild(Ref<Widget> child);

	sf::Vector2f getGlobalPosition() const;
	inline const sf::Vector2f& getRelativePosition() const { return m_RelativePosition; }

	// In principle, this is the only place where we will work with "true" position values (here we convert percentage positions into "true" positions for various widget drawable components). This work has to be done in each Widget-derived class which holds drawable component(s).
	virtual void setRelativePosition(const sf::Vector2f& pos);
	// We usually shouldn't have to override this function because it is defined in therms of setRelativePosition().
	virtual void setGlobalPosition(const sf::Vector2f& pos);

	// Sets viewport to this widget and all children. Viewport is a sf::FloatRect where values are between 0 and 1 : the edges of the window.
	void setViewport(const sf::FloatRect& viewport);

protected: // Protected methods
	/* Internal use only.
	 * These methods shall update/render/trigger onResize the current Widget only. Current/children Widget(s) updating/rendering/onResize triggering order is managed by update(const float&, CursorRay&), render(Ref<sf::RenderWindow>) and onResize(const sf::Vector2f&). */
	virtual void updateWidget(const float& dt, CursorRay& ray) {}
	virtual void renderWidget(Ref<sf::RenderWindow> window) {}
	virtual void onWidgetResize(const sf::Vector2f& size) {}

private: // Private methods
	// For more readable code in Widget class only, because they shouldn't be needed in Widget-derived classes.
	void updateChildren(const float& dt, CursorRay& ray);
	void renderChildren(Ref<sf::RenderWindow> window);
	void onResizeChildren(const sf::Vector2f& size);
	sf::IntRect getViewport() const;

private:
	Ref<Widget> m_Parent = nullptr;
	std::vector<Ref<Widget>> m_Children;

	Ref<sf::View> m_View;

	// Values bewteen 0 and 1 : the edges of the Widget's viewport. "True" position value is processed each time we move a Widget and given to SFML drawables. This gives an abstraction to the programmer to use Widgets with percentages. We don't want to bother with variable position values (because of resizing and flexible viewport size).
	sf::Vector2f m_RelativePosition;
};
