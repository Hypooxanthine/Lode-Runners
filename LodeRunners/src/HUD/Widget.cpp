#include "Widget.h"

Widget::Widget()
{
	onResize();
}

void Widget::update(const float& dt)
{
	static CursorRay ray; // Instanciated once.
	ray.create(*m_View); // Called each time.

	this->update(dt, ray);
}

// We must first update children, then update this. If children have something to do with a ray, they handle it and then parent don't. If they don't handle the ray, their parent can handle it. Recursive.
// Example : a Button widget has a Text widget as child. The Text widget first receives the CursorRay. It obviously has nothing to do with the ray, so it does not use it, and doesn't consume it. Then the Button widget handles the ray. If the ray is inside its bounding box, it consumes it, else, it does not consume it (eventually the ray can be used by the Button widget's parent).
void Widget::update(const float& dt, CursorRay& ray)
{
	updateChildren(dt, ray);
	updateWidget(dt, ray);
}

// We must first render "this", then children. Children have to show in front of their parent.
void Widget::render(Ref<sf::RenderWindow> window)
{
	renderWidget(window);
	renderChildren(window);
}

void Widget::onResize()
{
	const sf::IntRect viewport = getViewport();
	const sf::Vector2f size = { (float)viewport.width, (float)viewport.height };

	// We want the view's ratio to fit perfectly the viewport ratio.
	m_View->setCenter(size / 2.f);
	m_View->setSize(size);

	onResize(size);
}

// onResize order doesn't particularly counts.
void Widget::onResize(const sf::Vector2f& size)
{
	onWidgetResize(size);
	onResizeChildren(size);
}

inline void Widget::setParent(Ref<Widget> parent)
{
	m_Parent = parent;
	m_View = parent->m_View;
}

inline void Widget::addChild(Ref<Widget> child)
{
	m_Children.push_back(child);
	child->m_View = m_View;
}

inline sf::Vector2f Widget::getGlobalPosition() const
{
	if (m_Parent == nullptr) return m_RelativePosition;
	
	return m_Parent->getGlobalPosition() + m_RelativePosition;
}

inline void Widget::setRelativePosition(const sf::Vector2f& pos)
{
	m_RelativePosition = pos;
}

inline void Widget::setGlobalPosition(const sf::Vector2f& pos)
{
	if (m_Parent)
		setRelativePosition(pos - m_Parent->getGlobalPosition());
	else
		setRelativePosition(pos);
}

void Widget::updateChildren(const float& dt, CursorRay& ray)
{
	for (auto& child : m_Children)
		child->updateWidget(dt, ray);
}

void Widget::renderChildren(Ref<sf::RenderWindow> window)
{
	for (auto& child : m_Children)
		child->render(window);
}

void Widget::onResizeChildren(const sf::Vector2f& size)
{
	for (auto& child : m_Children)
		child->onResize(size);
}

inline sf::IntRect Widget::getViewport() const
{
	return Application::get()->getWindow()->getViewport(*m_View);
}

void Widget::setViewport(const sf::FloatRect& viewport)
{
	m_View->setViewport(viewport);
	onResize(); // Triggering onResize for this Widget and every children Widgets
}
