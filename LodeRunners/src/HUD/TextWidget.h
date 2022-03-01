#pragma once

#include "Widget.h"
#include "../Assets/FontAsset.h"

class TextWidget : public Widget
{
public:
	TextWidget();

	inline std::string getText() const { return m_FullText; }

	void setText(const std::string& text);
	inline void setColor(const sf::Color& col) { m_Text.setFillColor(col); }
	void setBold(const bool& val = true);
	void setUnderlined(const bool& val = true);
	void setItalic(const bool& val = true);
	inline void setWrapText(const bool& val = true) { m_WrapText = val; }

protected:
	//virtual void handleWidgetRay(CursorRay& ray) override;
	//virtual void updateWidget(const float& dt) override;
	virtual void renderWidget(Ref<sf::RenderWindow> window) override;
	virtual void onPositionUpdated() override;
	virtual void onSizeUpdated() override;

private: // Private methods
	void center();
	void wrap();

private: // Private members
	Ref<FontAsset> m_Font;
	sf::Text m_Text;
	// After wrapping, all the text may not be in sf::Text's string, so we need to store the full string somewhere.
	std::string m_FullText;

	bool m_WrapText = false;
};

