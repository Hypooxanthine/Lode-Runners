#pragma once

#include "Widgets.h"

class TextBoxWidget : public Widget
{
public:
	TextBoxWidget();
	TextBoxWidget(Widget* parent);

	inline void setTextColor(const sf::Color& col) { m_Text->setColor(col); }

	inline std::string getText() const { return m_Text->getText(); }
	inline void setText(const std::string& str) { m_Text->setText(str); }

protected: // Protected methods
	virtual void handleWidgetRay(CursorRay& ray) override;
	virtual void updateWidget(const float& dt) override;

private:
	Ref<ButtonWidget> m_Button;
	Ref<TextWidget> m_Text;

	bool m_HasFocus = false;
	bool m_WasPressed = false;
};

