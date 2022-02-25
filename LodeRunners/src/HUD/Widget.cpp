#include "Widget.h"

Widget::Widget()
{
}

const sf::Vector2f& Widget::getGlobalPosition() const
{
	if (m_Parent == nullptr) return m_RelativePosition;
	
	return m_RelativePosition + m_Parent->getGlobalPosition();
}
