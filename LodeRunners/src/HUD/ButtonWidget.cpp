#include "ButtonWidget.h"

ButtonWidget::ButtonWidget()
	: ButtonWidget({ 50, 50, 50 }, { 30, 30, 30 }, { 80, 80, 80 })
{}

ButtonWidget::ButtonWidget(const sf::Color& normalCol, const sf::Color& hoveredCol, const sf::Color& pressedCol)
	: Widget()
{
	m_Colors[ButtonProfile::Normal] = normalCol;
	m_Colors[ButtonProfile::Hovered] = hoveredCol;
	m_Colors[ButtonProfile::Pressed] = pressedCol;

	setButtonProfile(ButtonProfile::Normal);
}

void ButtonWidget::handleWidgetRay(CursorRay& ray)
{
	if (!ray)
	{
		if (m_Profile == ButtonProfile::Hovered || m_Profile == ButtonProfile::Pressed)
		{
			setButtonProfile(ButtonProfile::Normal);
			onStopHover();
		}
	}

	if (m_Surface.getGlobalBounds().contains(ray))
	{
		if (ray.isPressed())
		{
			if (m_Profile == ButtonProfile::Normal || m_Profile == ButtonProfile::Hovered)
			{
				setButtonProfile(ButtonProfile::Pressed);
				if(m_Profile == ButtonProfile::Normal) onHover();
				onPress();
			}
		}
		else
		{
			if (m_Profile == ButtonProfile::Normal)
			{
				setButtonProfile(ButtonProfile::Hovered);
				onHover();
			}
			else if (m_Profile == ButtonProfile::Pressed)
			{
				setButtonProfile(ButtonProfile::Hovered);
				onClick();
				for (auto& cb : m_Callbacks) cb();
			}
		}

		ray.consume();
	}
	else
	{
		if (m_Profile == ButtonProfile::Hovered || m_Profile == ButtonProfile::Pressed)
		{
			setButtonProfile(ButtonProfile::Normal);
			onStopHover();
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

void ButtonWidget::onSizeUpdated()
{
	m_Surface.setSize(getGlobalWorldSize());
}

void ButtonWidget::setButtonProfile(const ButtonProfile& profile)
{
	m_Profile = profile;
	m_Surface.setFillColor(m_Colors[profile]);
}
