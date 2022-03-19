#include "TextComponent.h"

#include "../../Assets/Assets.h"

TextComponent::TextComponent()
	: Component()
{
	m_Font = *Assets::getFontAsset();
	m_Text.setFont(m_Font);
}

void TextComponent::update(const float& dt)
{
}

void TextComponent::render(Ref<sf::RenderWindow> window)
{
	m_Text.setPosition(getWorldPosition() * SPACE_UNIT + m_PosCorrection);
	window->draw(m_Text);
}

std::string TextComponent::getText() const
{
	return m_Text.getString();
}

void TextComponent::setText(const std::string& text)
{
	m_Text.setString(text);
	updatePosCorrection();
}

unsigned int TextComponent::getCharacterSize() const
{
	return m_Text.getCharacterSize();
}

void TextComponent::setCharacterSize(const unsigned int& size)
{
	m_Text.setCharacterSize(size);
	updatePosCorrection();
}

sf::Color TextComponent::getColor() const
{
	return m_Text.getFillColor();
}

void TextComponent::setColor(const sf::Color& color)
{
	m_Text.setFillColor(color);
}

bool TextComponent::getBold() const
{
	return m_Text.getStyle() & sf::Text::Bold;
}

void TextComponent::setBold(const bool& val)
{
	if (val)
		m_Text.setStyle(m_Text.getStyle() | sf::Text::Bold);
	else
		m_Text.setStyle(m_Text.getStyle() & ~sf::Text::Bold);

	updatePosCorrection();
}

sf::Color TextComponent::getOutlineColor() const
{
	return m_Text.getOutlineColor();
}

void TextComponent::setOutlineColor(const sf::Color& color)
{
	m_Text.setOutlineColor(color);
}

float TextComponent::getOutlineThickness() const
{
	return m_Text.getOutlineThickness() / SPACE_UNIT;
}

void TextComponent::setOutlineThickness(const float& val)
{
	m_Text.setOutlineThickness(val * SPACE_UNIT);
}

const TextComponent::HorizontalAlign& TextComponent::getHorizontalAlign() const
{
	return m_HAlign;
}

void TextComponent::setHorizontalAlign(const HorizontalAlign& align)
{
	m_HAlign = align;
	updatePosCorrection();
}

const TextComponent::VerticalAlign& TextComponent::getVerticalAlign() const
{
	return m_VAlign;
}

void TextComponent::setVerticalAlign(const VerticalAlign& align)
{
	m_VAlign = align;
	updatePosCorrection();
}

void TextComponent::updatePosCorrection()
{
	auto bounds = m_Text.getGlobalBounds();

	switch (m_HAlign)
	{
	case HorizontalAlign::Left:
		m_PosCorrection.x = 0.f;
		break;
	case HorizontalAlign::Center:
		m_PosCorrection.x = -bounds.width / 2.f;
		break;
	case HorizontalAlign::Right:
		m_PosCorrection.x = -bounds.width;
		break;
	default:
		break;
	}

	switch (m_VAlign)
	{
	case VerticalAlign::Top:
		m_PosCorrection.y = 0.f;
		break;
	case VerticalAlign::Middle:
		m_PosCorrection.y = -bounds.height / 2.f;
		break;
	case VerticalAlign::Bottom:
		m_PosCorrection.y = -bounds.height;
		break;
	default:
		break;
	}
}
