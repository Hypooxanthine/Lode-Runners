#include "TextWidget.h"
#include "../Assets/Assets.h"

TextWidget::TextWidget()
{
	// xmemory exception when static FontAsset (from Assets class) is given to sf::Text::setFont(const sf::Font&). I don't really know why. Have to check SFML source to see if there is any problem with static references to sf::Font.
	// This solution implies a loss of performance. Need to figure it out.
	m_Font = MakeRef<FontAsset>(*Assets::getFontAsset());
	m_Text.setFont(*m_Font);
}

void TextWidget::setText(const std::string& text)
{
	m_Text.setString(text);
	m_FullText = text;
	updateText();
}

void TextWidget::setBold(const bool& val)
{
	if (val)
		m_Text.setStyle(m_Text.getStyle() | sf::Text::Bold);
	else
		m_Text.setStyle(m_Text.getStyle() & ~sf::Text::Bold);
	updateText();
}

void TextWidget::setUnderlined(const bool& val)
{
	if (val)
		m_Text.setStyle(m_Text.getStyle() | sf::Text::Underlined);
	else
		m_Text.setStyle(m_Text.getStyle() & ~sf::Text::Underlined);
	updateText();
}

void TextWidget::setItalic(const bool& val)
{
	if (val)
		m_Text.setStyle(m_Text.getStyle() | sf::Text::Italic);
	else
		m_Text.setStyle(m_Text.getStyle() & ~sf::Text::Italic);
	updateText();
}

void TextWidget::renderWidget(Ref<sf::RenderWindow> window)
{
	try {
		window->draw(m_Text);
	}
	catch (std::exception e)
	{
		LOG_ERROR(e.what());
	}
}

void TextWidget::onPositionUpdated()
{
	updateText();
}

void TextWidget::onSizeUpdated()
{
	updateText();
}

void TextWidget::updateText()
{
	//m_Text.setCharacterSize((int)(30.f * (float)Application::get()->getWindow()->getSize().x / 1920.f));

	if (m_WrapText)
		wrap();
	if (m_CenterX)
		centerX();
	if (m_CenterY)
		centerY();
}

void TextWidget::centerX()
{
	const auto pos = getGlobalWorldPosition();
	const auto size = getGlobalWorldSize();
	const auto textRect = m_Text.getLocalBounds();

	// sf::Text bounds are not exactly placed at its position, so we have to fix this little difference ourselves.
	m_Text.setPosition
	(
		pos.x + (size.x - textRect.width) / 2.f - textRect.left,
		m_Text.getPosition().y
	);
}

void TextWidget::centerY()
{
	const auto pos = getGlobalWorldPosition();
	const auto size = getGlobalWorldSize();
	const auto textRect = m_Text.getLocalBounds();

	// sf::Text bounds are not exactly placed at its position, so we have to fix this little difference ourselves.
	m_Text.setPosition
	(
		m_Text.getPosition().x,
		pos.y + (size.y - textRect.height) / 2.f - textRect.top
	);
}

// Wraps the "str" string, and returns the remaining str.
std::string WrapLine(std::string& str, const sf::Text& ref, const float& maxWidth)
{
	std::stringstream ss(str);
	str = "";

	sf::Text lineText = sf::Text(ref);
	lineText.setString("");

	bool end = false;
	while (!end)
	{
		std::string word;
		ss >> word,
		lineText.setString(str + word);

		if (lineText.getGlobalBounds().width > maxWidth || ss.str().length() == 0)
		{
			ss << word;
			end = true;
		}
		else
		{
			str += " " + word;
		}
	}

	return ss.str();

}

// TODO : make it work.
void TextWidget::wrap()
{
	const sf::FloatRect box(getGlobalWorldPosition(), getGlobalWorldSize());

	std::string str = m_FullText;
	std::string rest;

	bool end = false;

	while (!end)
	{
		rest = WrapLine(str, m_Text, box.width);
		sf::Text temp = sf::Text(m_Text);

		temp.setString(temp.getString() + str);

		if (rest.size() > 0 && m_Text.getGlobalBounds().height < box.height)
		{
			str = rest;
			m_Text.setString(temp.getString() + "\n");
		}
		else
		{
			end = true;
		}
	} 
}
