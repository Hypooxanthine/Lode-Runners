#include "TextButtonWidget.h"

TextButtonWidget::TextButtonWidget()
	: TextButtonWidget(nullptr, "")
{
}

TextButtonWidget::TextButtonWidget(const std::string& text)
	: TextButtonWidget(nullptr, text)
{
}

TextButtonWidget::TextButtonWidget(Widget* parent)
	: TextButtonWidget(parent, "")
{

}

TextButtonWidget::TextButtonWidget(Widget* parent, const std::string& text)
	: ButtonWidget(parent)
{
	m_TextWidget = MakeRef<TextWidget>(this);
	m_TextWidget->fillParent();
	m_TextWidget->setText(text);
}
