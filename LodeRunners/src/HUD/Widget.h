#pragma once

#include "../Core/Base.h"

class CursorRay
{
public:
	CursorRay() = default;

	void create(const sf::View& view)
	{
		m_Consumed = false;
		const auto window = Application::get()->getWindow();
		const auto mousePos = sf::Mouse::getPosition(*window);
		// Converting screen mouse position into widget's "true" coordinate system.
		m_Position = window->mapPixelToCoords(mousePos, view);
		m_Pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	inline void consume() { m_Consumed = true; }
	inline const bool& isConsumed() const { return m_Consumed; }
	// if(CursorRay) -> condition is true if the ray has not been consumed yet.
	operator bool() const { return !m_Consumed; }

	const sf::Vector2f& getPosition() const { return m_Position; }
	const bool& isPressed() const { return m_Pressed; }
private:
	// When a CursorRay is used (example : it hovers a button, so the button changes his color and consumes the ray), the widget that uses it has to make this value to true. No verification is done when trying to access data. A widget can see data even if its m_Consumed member is true. It is its own responsability to deal with this piece of information. Is there any way to make it safer without reducing performance ? Conceptually, there is probably no way to determine if a widgets really "consumes" a ray or not.
	bool m_Consumed = false;
	// Position is "world" position. This means the position is in the widget's "true" coordinate systems (this is NOT values between 0 and 1). It can directly be used with sf::Rect<T>::contains(const sf::Vector2<T>&) to check if the cursor hovers a widget bounding box (= hitbox).
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
