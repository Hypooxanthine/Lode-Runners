#include "ButtonWidget.h"

ButtonWidget::ButtonWidget()
{
	m_Surface.setFillColor(sf::Color::Red);
	m_Surface.setSize({ 50.f, 50.f });
}

void ButtonWidget::handleWidgetRay(CursorRay& ray)
{
	if (ray)
	{
		if (m_Surface.getGlobalBounds().contains(ray.getPosition()))
		{
			LOG_TRACE("Hovered.");
		}
		else
		{
			LOG_TRACE("Not hovered.");
		}
	}
}

void ButtonWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	window->draw(m_Surface);
}

void ButtonWidget::onPositionUpdated()
{
	m_Surface.setPosition(getGlobalWorldPosition());
}
