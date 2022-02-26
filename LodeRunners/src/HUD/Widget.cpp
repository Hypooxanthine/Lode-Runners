#include "Widget.h"

void Widget::update(const float& dt)
{
	static CursorRay ray; // Instanciated once.
	ray.create(*m_View); // Called each time.

	this->handleRays(ray);
	this->updateWidgets(dt);
}

// We must first render "this", then children. Children have to show in front of their parent.
void Widget::render(Ref<sf::RenderWindow> window)
{
	window->setView(*m_View);
	renderWidget(window);
	renderChildren(window);
	window->setView(window->getDefaultView());
}

void Widget::onResize()
{
	const sf::IntRect viewport = getViewport();
	const sf::Vector2f size = { (float)viewport.width, (float)viewport.height };

	// We want the view's ratio to fit perfectly the viewport ratio. m_View is a ref, so it changes for all children.
	m_View->setCenter(size / 2.f);
	m_View->setSize(size);

	updatePositions();
}

void Widget::setParent(Ref<Widget> parent)
{
	m_Parent = parent;
	m_View->setCenter(parent->m_View->getCenter());
	m_View->setSize(parent->m_View->getSize());
	m_View->setViewport(parent->m_View->getViewport());
	updatePositions();
}

void Widget::addChild(Ref<Widget> child)
{
	m_Children.push_back(child);
	child->m_View->setCenter(m_View->getCenter());
	child->m_View->setSize(m_View->getSize());
	child->m_View->setViewport(m_View->getViewport());
	child->updatePositions();
}

sf::Vector2f Widget::getGlobalPosition() const
{
	if (m_Parent == nullptr) return m_RelativePosition;
	
	return m_Parent->getGlobalPosition() + m_RelativePosition;
}

sf::Vector2f Widget::getGlobalWorldPosition() const
{
	const auto viewport = getViewport();
	const auto globalPos = getGlobalPosition();

	return { m_View->getSize().x * globalPos.x, m_View->getSize().y * globalPos.y };
}

void Widget::setRelativePosition(const sf::Vector2f& pos)
{
	m_RelativePosition = pos;
	updatePositions();
}

void Widget::setGlobalPosition(const sf::Vector2f& pos)
{
	if (m_Parent)
		setRelativePosition(pos - m_Parent->getGlobalPosition());
	else
		setRelativePosition(pos);
}

void Widget::updateChildren(const float& dt)
{
	for (auto& child : m_Children)
		child->updateWidget(dt);
}

void Widget::handleRayChildren(CursorRay& ray)
{
	for (auto& child : m_Children)
		child->handleRays(ray);
}

void Widget::renderChildren(Ref<sf::RenderWindow> window)
{
	for (auto& child : m_Children)
		child->render(window);
}

sf::IntRect Widget::getViewport() const
{
	return Application::get()->getWindow()->getViewport(*m_View);
}

void Widget::handleRays(CursorRay& ray)
{
	handleRayChildren(ray);
	handleWidgetRay(ray);
}

// We must first update children, then update this. If children have something to do with a ray, they handle it and then parent don't. If they don't handle the ray, their parent can handle it. Recursive.
// Example : a Button widget has a Text widget as child. The Text widget first receives the CursorRay. It obviously has nothing to do with the ray, so it does not use it, and doesn't consume it. Then the Button widget handles the ray. If the ray is inside its bounding box, it consumes it, else, it does not consume it (eventually the ray can be used by the Button widget's parent).
void Widget::updateWidgets(const float& dt)
{
	updateChildren(dt);
	updateWidget(dt);
}

// We must first update "this" position, then children's positions, because their positions are relative to "this" position.
void Widget::updatePositions()
{
	onPositionUpdated();

	for (auto& child : m_Children)
		child->updatePositions();
}

void Widget::setViewport(const sf::FloatRect& viewport)
{
	m_View->setViewport(viewport);
	onResize(); // Triggering onResize for this Widget and every children Widgets
}
