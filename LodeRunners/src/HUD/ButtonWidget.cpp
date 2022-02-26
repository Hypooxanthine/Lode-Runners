#include "ButtonWidget.h"

ButtonWidget::ButtonWidget()
{
	
}

void ButtonWidget::handleWidgetRay(CursorRay& ray)
{
	if (ray)
	{
		if (m_Surface.getGlobalBounds().contains(ray.getPosition()))
		{
			m_Surface.setFillColor(sf::Color::Green);
			ray.consume();
		}
		else
		{
			m_Surface.setFillColor(originalColor);
		}
	}
	else
		m_Surface.setFillColor(originalColor);
}

void ButtonWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	window->draw(m_Surface);
}

void ButtonWidget::onPositionUpdated()
{
	m_Surface.setPosition(getGlobalWorldPosition());
	LOG_TRACE(std::string("Position : ") + std::to_string(m_Surface.getPosition().x) + ", "
		+ std::to_string(m_Surface.getPosition().y));
}

void ButtonWidget::onSizeUpdated()
{
	m_Surface.setSize(getGlobalWorldSize());
	LOG_TRACE(std::string("Size : ") + std::to_string(m_Surface.getSize().x) + ", "
		+ std::to_string(m_Surface.getSize().y));
}
