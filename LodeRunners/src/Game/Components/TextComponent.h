#pragma once

#include "Component.h"

class TextComponent : public Component
{
public:
	enum class HorizontalAlign { Left, Center, Right };
	enum class VerticalAlign { Top, Middle, Bottom };
public:
	TextComponent();

	virtual void update(const float& dt);
	virtual void render(Ref<sf::RenderWindow> window);

	void setRender(const bool& val) { m_Render = val; }
	const bool& getRender() const { return m_Render; }

	std::string getText() const;
	void setText(const std::string& text);

	unsigned int getCharacterSize() const;
	void setCharacterSize(const unsigned int& size);

	sf::Color getColor() const;
	void setColor(const sf::Color& color);

	bool getBold() const;
	void setBold(const bool& val = true);

	sf::Color getOutlineColor() const;
	void setOutlineColor(const sf::Color& color);

	float getOutlineThickness() const;
	void setOutlineThickness(const float& val);

	const HorizontalAlign& getHorizontalAlign() const;
	void setHorizontalAlign(const HorizontalAlign& align);

	const VerticalAlign& getVerticalAlign() const;
	void setVerticalAlign(const VerticalAlign& align);

private: // Private member functions
	void updatePosCorrection();

private: // Private members
	sf::Text m_Text;
	sf::Font m_Font;

	bool m_Render = true;

	HorizontalAlign m_HAlign = HorizontalAlign::Left;
	VerticalAlign m_VAlign = VerticalAlign::Top;

	sf::Vector2f m_PosCorrection;
};

