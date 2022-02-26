#pragma once

#include "Widget.h"

class ButtonWidget : public Widget
{
public:
	ButtonWidget();

protected: // Protected methods
	virtual void handleWidgetRay(CursorRay& ray);
	virtual void renderWidget(Ref<sf::RenderWindow> window);
	virtual void onPositionUpdated();

private:
	sf::RectangleShape m_Surface;
};

