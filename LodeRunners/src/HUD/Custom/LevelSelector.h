#pragma once
#include "../Widgets.h"

class LevelSelector : public Widget
{
public:
	enum class Style { Normal, Loaded, Edited };
public:
	LevelSelector();
	LevelSelector(Widget* parent);

	//const size_t& getSelectedLevel() const { return m_SelectedLevel; }
	std::string getSelectedLevelName() const { return m_LevelNameText->getText(); }

	void setStyle(const Style& style) { m_Style = style; updateStyle(); }

private: // Private methods
	// Callbacks
	void previousLevel();
	void nextLevel();
	void updateStyle();

private: // Private members
	Ref<TextWidget> m_LevelNameText;
	Ref<ButtonWidget> m_PreviousLevelButton;
	Ref<ButtonWidget> m_NextLevelButton;

	size_t m_SelectedLevel = 0;
	Style m_Style = Style::Normal;
};

