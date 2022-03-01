#pragma once

#include "TextWidget.h"
#include "ButtonWidget.h"
class TextBoxWidget : public Widget
{
public:
	TextBoxWidget();

	inline void setBackgroundColor(const sf::Color& col) { m_Shape.setFillColor(col); }
	inline void setTextColor(const sf::Color& col) { m_Text.setFillColor(col); }

	inline std::string getText() const { return m_Text.getString(); }
	inline void setText(const std::string& str) { m_Text.setString(str); }

protected: // Protected methods
	virtual void handleWidgetRay(CursorRay& ray) override;
	virtual void updateWidget(const float& dt) override;
	virtual void renderWidget(Ref<sf::RenderWindow> window) override;
	virtual void onPositionUpdated() override;
	virtual void onSizeUpdated() override;

	void centerText();

private:
	sf::RectangleShape m_Shape;
	Ref<FontAsset> m_Font;
	sf::Text m_Text;
	sf::RectangleShape m_Blinker;

	bool m_HasFocus = false;
};

