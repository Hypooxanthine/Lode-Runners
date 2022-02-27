#pragma once

#include "Widget.h"
#include "../Assets/FontAsset.h"

class TextWidget : public Widget
{
public:
	TextWidget();

	void setText(const std::string& text);

protected:
	//virtual void handleWidgetRay(CursorRay& ray) override;
	//virtual void updateWidget(const float& dt) override;
	virtual void renderWidget(Ref<sf::RenderWindow> window) override;
	virtual void onPositionUpdated() override;
	virtual void onSizeUpdated() override;

private: // Private methods
	void center();

private: // Private members
	Ref<FontAsset> m_Font;
	sf::Text m_Text;
};

