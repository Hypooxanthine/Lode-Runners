#include "TextWidget.h"
#include "../Assets/Assets.h"

TextWidget::TextWidget()
{
	// xmemory exception when static FontAsset (from Assets class) is given to sf::Text::setFont(const sf::Font&). I don't really know why. Have to check SFML source to see if there is any problem with static references to sf::Font.
	// This solution is a huge loss of performance. Need to figure it out.
	m_Font = MakeRef<FontAsset>(*Assets::getFontAsset());
	m_Text.setFont(*m_Font);
}

void TextWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	window->draw(m_Text);
}

void TextWidget::onPositionUpdated()
{
	center();
}

void TextWidget::onSizeUpdated()
{
	center();
}

void TextWidget::center()
{
	const auto requestedRect = sf::FloatRect(getGlobalWorldPosition(), getGlobalWorldSize());
	const auto textRect = m_Text.getGlobalBounds();

	m_Text.setPosition
	(
		requestedRect.left + (requestedRect.width - textRect.width) / 2.f,
		requestedRect.top + (requestedRect.height - textRect.height) / 2.f
	);
}
