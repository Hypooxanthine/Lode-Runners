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
	updateSizes();
}

/* HIERARCHY */

void Widget::addChild(Ref<Widget> child, Ref<Widget> parent)
{
	parent->m_Children.push_back(child);
	child->m_Parent = parent;
	child->m_View = parent->m_View;
	child->setChildrenView(parent->m_View);
	child->updatePositions();
	child->updateSizes();
}

bool Widget::removeChild(Ref<Widget> child)
{
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] == child)
		{
			m_Children[i]->m_Parent = nullptr;
			m_Children.erase(m_Children.begin() + i);
			return true;
		}
	}
		return false;
}

void Widget::removeChildren()
{
	for (auto& c : m_Children)
		c->m_Parent = nullptr;
	m_Children.clear();
}

sf::Vector2f Widget::getGlobalPosition() const
{
	if (m_Parent == nullptr) return { m_RelativeRect.left, m_RelativeRect.top };

	const auto parentGlobalSize = m_Parent->getGlobalSize();

	return m_Parent->getGlobalPosition() + sf::Vector2f
	(
		m_RelativeRect.left * parentGlobalSize.x,
		m_RelativeRect.top * parentGlobalSize.y
	);
}

sf::Vector2f Widget::getGlobalWorldPosition() const
{
	const auto viewport = getViewport();
	const auto globalPos = getGlobalPosition();

	return { m_View->getSize().x * globalPos.x, m_View->getSize().y * globalPos.y };
}

void Widget::setRelativePosition(const sf::Vector2f& pos)
{
	m_RelativeRect.left = pos.x;
	m_RelativeRect.top = pos.y;
	updatePositions();
}

void Widget::setGlobalPosition(const sf::Vector2f& pos)
{
	if (m_Parent)
		setRelativePosition(pos - m_Parent->getGlobalPosition());
	else
		setRelativePosition(pos);
}

/* Sizes */

sf::Vector2f Widget::getGlobalSize() const
{
	if (m_Parent == nullptr) return { m_RelativeRect.width, m_RelativeRect.height };
	
	const auto parentGlobalSize = m_Parent->getGlobalSize();
	return { m_RelativeRect.width * parentGlobalSize.x, m_RelativeRect.height * parentGlobalSize.y };
}

sf::Vector2f Widget::getGlobalWorldSize() const
{
	const auto viewport = getViewport();
	const auto globalSize = getGlobalSize();

	return { m_View->getSize().x * globalSize.x, m_View->getSize().y * globalSize.y };
}

void Widget::setRelativeSize(const sf::Vector2f& size)
{
	m_RelativeRect.width = size.x;
	m_RelativeRect.height = size.y;
	updateSizes();
}

void Widget::setGlobalSize(const sf::Vector2f& size)
{
	if (m_Parent)
	{
		const auto parentGlobalSize = m_Parent->getGlobalSize();
		setRelativeSize({ size.x / parentGlobalSize.x, size.y / parentGlobalSize.y });
	}
	else
		setRelativeSize(size);
}

/* Utility */

void Widget::fillParent()
{
	setRelativePosition({ 0.f, 0.f });
	setRelativeSize({ 1.f, 1.f });
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

void Widget::updateChildrenPosition()
{
	for (auto& child : m_Children)
		child->updatePositions();
}

void Widget::updateChildrenSize()
{
	for (auto& child : m_Children)
		child->updateSizes();
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
	updateChildrenPosition();
}

void Widget::updateSizes()
{
	onSizeUpdated();
	updateChildrenSize();
}

void Widget::setChildrenView(Ref<sf::View> view)
{
	for (auto& child : m_Children)
	{
		child->m_View = view;
		child->setChildrenView(view);
	}
}

void Widget::setViewport(const sf::FloatRect& viewport)
{
	m_View->setViewport(viewport);
	onResize(); // Triggering onResize for this Widget and every children Widgets
}
