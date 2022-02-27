#pragma once

#include "Widget.h"

class ButtonWidget : public Widget
{
public: // Public methods
	ButtonWidget();
	ButtonWidget(const sf::Color& normalCol, const sf::Color& hoveredCol, const sf::Color& pressedCol);

	inline void setNormalColor(const sf::Color& col)  { m_Colors[ButtonProfile::Normal]  = col; }
	inline void setHoveredColor(const sf::Color& col) { m_Colors[ButtonProfile::Hovered] = col; }
	inline void setPressedColor(const sf::Color& col) { m_Colors[ButtonProfile::Pressed] = col; }

	inline const sf::Color& getNormalColor() const  { return m_Colors.at(ButtonProfile::Normal); }
	inline const sf::Color& getHoveredColor() const { return m_Colors.at(ButtonProfile::Hovered); }
	inline const sf::Color& getPressedColor() const { return m_Colors.at(ButtonProfile::Pressed); }

	inline void bindCallback(const Callback& callback) { m_Callbacks.push_back(callback); }
	inline void operator+=(const Callback& callback) { bindCallback(callback); }

protected: // Protected methods
	virtual void handleWidgetRay(CursorRay& ray) override;
	virtual void renderWidget(Ref<sf::RenderWindow> window) override;
	virtual void onPositionUpdated() override;
	virtual void onSizeUpdated() override;

	virtual void onClick() {}
	virtual void onPress() {}
	virtual void onRelease() {}
	virtual void onHover() {}
	virtual void onStopHover() {}

private: // Private structures
	enum class ButtonProfile { Normal, Hovered, Pressed };

private: // Private methods
	void setButtonProfile(const ButtonProfile& profile);

private: // Private members
	sf::RectangleShape m_Surface;
	std::map<ButtonProfile, sf::Color> m_Colors;
	ButtonProfile m_Profile = ButtonProfile::Normal;

	std::vector<Callback> m_Callbacks;
};

