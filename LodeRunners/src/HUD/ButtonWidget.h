#pragma once

#include "Widget.h"

class ButtonWidget : public Widget
{
public:
	ButtonWidget();

	inline void setFillColor(const sf::Color& col) { m_Surface.setFillColor(col); originalColor = col; }

protected: // Protected methods
	virtual void handleWidgetRay(CursorRay& ray);
	virtual void renderWidget(Ref<sf::RenderWindow> window);
	virtual void onPositionUpdated();
	virtual void onSizeUpdated();

private:
	sf::RectangleShape m_Surface;
	sf::Color originalColor;
};

