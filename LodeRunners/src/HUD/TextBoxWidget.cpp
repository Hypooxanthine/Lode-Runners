#include "TextBoxWidget.h"
#include "../Assets/Assets.h"

TextBoxWidget::TextBoxWidget()
{
	m_Font = MakeRef<FontAsset>(*Assets::getFontAsset());
	m_Text.setFont(*m_Font);
	this->setTextColor(sf::Color::Black);
	m_Blinker.setFillColor(sf::Color::Black);
}

void TextBoxWidget::handleWidgetRay(CursorRay& ray)
{
	static bool wasPressed = false;
	const sf::FloatRect buttonSurface = sf::FloatRect(getGlobalWorldPosition(), getGlobalWorldSize());

	// If we just press anywhere but in the button rect, TextBox releases focus.
	if (!wasPressed && ray.isPressed())
	{
		if (buttonSurface.contains(ray))
			m_HasFocus = true;
		else
			m_HasFocus = false;
	}

	wasPressed = ray.isPressed();
}

void TextBoxWidget::updateWidget(const float& dt)
{
	if (m_HasFocus)
	{
		const Event& textEnteredEvent = Application::get()->getEvent(EventType::TextEntered);

		if (textEnteredEvent)
		{
			m_Text.setString(m_Text.getString() + textEnteredEvent.text);
			centerText();
		}

		const Event& textErasedEvent = Application::get()->getEvent(EventType::TextErased);

		if (textErasedEvent)
		{
			std::string t = m_Text.getString();
			if (t.size() > 0)
				t.erase(t.size() - 1);

			m_Text.setString(t);
			centerText();
		}
	}
}

void TextBoxWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	static sf::Clock clock;

	window->draw(m_Shape);
	window->draw(m_Text);

	if (m_HasFocus)
	{
		if (clock.getElapsedTime().asSeconds() > .5f)
		{
			window->draw(m_Blinker);
			if (clock.getElapsedTime().asSeconds() > 1.f)
				clock.restart();
		}
	}
}

void TextBoxWidget::onPositionUpdated()
{
	m_Shape.setPosition(getGlobalWorldPosition());
	centerText();
}

void TextBoxWidget::onSizeUpdated()
{
	m_Shape.setSize(getGlobalWorldSize());
	centerText();
}

void TextBoxWidget::centerText()
{
	const auto pos = getGlobalWorldPosition();
	const auto size = getGlobalWorldSize();
	const auto textRect = m_Text.getLocalBounds();

	// sf::Text bounds are not exactly placed at its position, so we have to fix this little difference ourselves.
	m_Text.setPosition
	(
		pos.x + (size.x - textRect.width) / 2.f - textRect.left,
		pos.y + (size.y - textRect.height) / 2.f - textRect.top
	);

	const auto globalTextRect = m_Text.getGlobalBounds();
	m_Blinker.setSize({ (float)m_Text.getCharacterSize() / 8.f, textRect.height });
	m_Blinker.setPosition({ globalTextRect.left + textRect.width + m_Blinker.getSize().x, globalTextRect.top });
}
