#include "TextBoxWidget.h"
#include "../Assets/Assets.h"

TextBoxWidget::TextBoxWidget()
	: TextBoxWidget(nullptr)
{}

TextBoxWidget::TextBoxWidget(Widget* parent)
	: Widget(parent)
{
	m_Button = MakeRef<ButtonWidget>(this, sf::Color(255, 255, 255), sf::Color(200, 200, 200), sf::Color(220, 220, 220));
	m_Text = MakeRef<TextWidget>(m_Button.get());

	m_Button->fillParent();
	m_Text->fillParent();

	m_Button->bindCallback([this]() {m_HasFocus = true; });

	setTextColor(sf::Color::Black);
}

void TextBoxWidget::handleWidgetRay(CursorRay& ray)
{
	const sf::FloatRect buttonSurface = sf::FloatRect(getGlobalWorldPosition(), getGlobalWorldSize());

	// If we just press anywhere but in the button rect, TextBox releases focus.
	if (!m_WasPressed 
		&& ray.isPressed() 
		&& !buttonSurface.contains(ray))
			m_HasFocus = false;

	m_WasPressed = ray.isPressed();
}

void TextBoxWidget::updateWidget(const float& dt)
{
	if (m_HasFocus)
	{
		const Event& textEnteredEvent = Application::get()->getEvent(EventType::TextEntered);

		if (textEnteredEvent)
			setText(getText() + textEnteredEvent.text);

		const Event& textErasedEvent = Application::get()->getEvent(EventType::TextErased);

		if (textErasedEvent)
		{
			std::string t = getText();
			if (t.size() > 0)
				t.erase(t.size() - 1);

			setText(t);
		}
	}
}