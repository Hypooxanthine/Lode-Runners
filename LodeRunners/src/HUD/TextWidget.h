#pragma once

#include "Widget.h"
#include "../Assets/FontAsset.h"

class TextWidget : public Widget
{
public:
	TextWidget();

	inline void setText(const std::string& text) { m_Text.setString(text); }

protected:
	//virtual void handleWidgetRay(CursorRay& ray) override;
	//virtual void updateWidget(const float& dt) override;
	virtual void renderWidget(Ref<sf::RenderWindow> window) override;
	virtual void onPositionUpdated() override;
	virtual void onSizeUpdated() override;

private: // Private methods

private: // Private members
	sf::Text m_Text;
	sf::RenderTexture m_RendTex;
	sf::Sprite m_Sprite;
};

