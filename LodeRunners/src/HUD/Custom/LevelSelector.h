#pragma once
#include "../Widgets.h"

class LevelSelector : public Widget
{
public:
	LevelSelector();
	LevelSelector(Widget* parent);

	size_t getSelectedLevelIndex() const { return m_SelectedLevel; }
	std::string getSelectedLevelName() const { return m_LevelNameText->getText(); }
	void setSelectedLevelIndex(const size_t& val) { m_SelectedLevel = val; updateStyle(); }

	const bool& isEdited() const { return m_Edited; }
	void setEdited(const bool& val) { m_Edited = val; updateStyle(); }

	const std::optional<size_t>& getLoadedLevel() const { return m_LoadedLevel; }
	void setLoadedLevel(const size_t& val) { m_LoadedLevel = val; updateStyle(); }
	void setLoadedLevel() { setLoadedLevel(m_SelectedLevel); }

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
	std::optional<size_t> m_LoadedLevel;
	bool m_Edited = false;
};

