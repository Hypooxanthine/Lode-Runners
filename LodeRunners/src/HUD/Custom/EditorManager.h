#pragma once

#include "../Widgets.h"
#include "LevelSelector.h"

#include "../../Assets/LevelAsset.h"

class EditorManager : public Widget
{
public:
	EditorManager(Ref<LevelAsset>& levelRef);

	void onEdited() { m_LevelSelector->setEdited(true); }

private: // Private methods
	// Callbacks
	void loadLevel();
	void saveLevel();
	void clearLevel();
	void createLevel();

private: // Private members
	Ref<TextButtonWidget> m_QuitButton;

	Ref<LevelSelector> m_LevelSelector;
	Ref<TextButtonWidget> m_LoadButton;
	Ref<TextButtonWidget> m_SaveButton;

	Ref<TextButtonWidget> m_ClearButton;

	Ref<TextBoxWidget> m_CreateLevelBox;
	Ref<TextButtonWidget> m_CreateButton;

	Ref<LevelAsset>& m_LevelRef;
};

