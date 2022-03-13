#pragma once

#include "Widgets.h"

class TextButtonWidget : public ButtonWidget
{
public:
	TextButtonWidget();
	TextButtonWidget(const std::string& text);
	TextButtonWidget(Widget* parent);
	TextButtonWidget(Widget* parent, const std::string& text);

private:
	Ref<TextWidget> m_TextWidget;
};

