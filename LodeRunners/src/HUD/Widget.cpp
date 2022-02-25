#include "Widget.h"

Widget::Widget()
{
}

void Widget::update(const float& dt)
{
	CursorRay ray;
	this->update(dt, ray);
}

const sf::Vector2f& Widget::getGlobalPosition() const
{
	if (m_Parent == nullptr) return m_RelativePosition;
	
	return m_RelativePosition + m_Parent->getGlobalPosition();
}

void Widget::updateChildren(const float& dt, CursorRay& ray)
{
	for (auto& child : m_Children)
		child->update(dt, ray);
}

void Widget::renderChildren(Ref<sf::RenderWindow> window)
{
	for (auto& child : m_Children)
		child->render(window);
}
