#include "TextWidget.h"
#include "../Assets/Assets.h"

TextWidget::TextWidget()
{
	m_Sprite.setTexture(m_RendTex.getTexture());
	m_Text.setFont(*Assets::getFontAsset());
}

void TextWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	window->draw(m_Sprite);
}

void TextWidget::onPositionUpdated()
{
	m_Text.setPosition(getGlobalWorldPosition());
}

void TextWidget::onSizeUpdated()
{
	const auto requestedSize = getGlobalWorldSize();

	m_RendTex.create((unsigned int)requestedSize.x, (unsigned int)requestedSize.y);
	m_RendTex.clear(sf::Color::Transparent);
	auto temp = sf::Text(m_Text);
	temp.setPosition(0.f, 0.f);
	m_RendTex.draw(temp);
	m_RendTex.display();
}