#pragma once
#include "States.h"
#include "../Assets/Assets.h"
#include "../HUD/Widget.h"
#include "../HUD/TextWidget.h"
#include "../HUD/TextBoxWidget.h"

class LevelAsset;

class EditorToolkit : State
{
public:
	EditorToolkit();

	inline Ref<const SpriteAsset> getSelectedTile() const { return m_SelectedTile; }

	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

private: // Private methods
	void select(Ref<SpriteAsset> tile);
	void hover(Ref<SpriteAsset> tile);

	Ref<SpriteAsset> getHoveredTile();

private: // Private members
	sf::View m_ToolkitView;

	std::vector<Ref<SpriteAsset>> m_Tiles;
	
	Ref<SpriteAsset> m_HoveredTile = nullptr;
	sf::RectangleShape m_HoveredHighlight;
	bool m_RenderHoveredHighlight = false;

	Ref<SpriteAsset> m_SelectedTile = nullptr;
	sf::RectangleShape m_SelectedHighlight;
};

class EditorUI
{
public:
	EditorUI(Ref<LevelAsset>& levelRef);

	void update(const float& dt);
	void render(Ref<sf::RenderWindow> window);
	void onResize();
	void onEdited();

private: // Private methods
	// Callbacks
	void previousLevel();
	void nextLevel();
	void updateSelectorStyle();

	void loadLevel();
	void saveLevel();
	void clearLevel();
	void createLevel();

private: // Private members
	Ref<Widget> m_HUD;
	Ref<TextWidget> m_LevelSelector;
	Ref<TextBoxWidget> m_CreateLevelBox;
	Ref<ButtonWidget> m_QuitButton;
	Ref<TextWidget> m_QuitText;
	Ref<ButtonWidget> m_PreviousLevelButton;
	Ref<TextWidget> m_PreviousLevelText;
	Ref<ButtonWidget> m_NextLevelButton;
	Ref<TextWidget> m_NextLevelText;
	Ref<ButtonWidget> m_LoadButton;
	Ref<TextWidget> m_LoadText;
	Ref<ButtonWidget> m_SaveButton;
	Ref<TextWidget> m_SaveText;
	Ref<ButtonWidget> m_ClearButton;
	Ref<TextWidget> m_ClearText;
	Ref<ButtonWidget> m_CreateButton;
	Ref<TextWidget> m_CreateText;

	size_t m_SelectedLevel;
	size_t m_LoadedLevel;
	// When a loaded level is edited, we need to know it to change its color, letting the user know its work has not been saved yet.
	bool m_Edited = false;

	Ref<LevelAsset>& m_LevelRef;

};

class EditorState : public State
{
public:
	EditorState();

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

private:
	EditorToolkit m_Toolkit;
	EditorUI m_UI;

	Ref<LevelAsset> m_LevelAsset;

	sf::View m_LevelView;

	bool m_RenderHighlighter = false;
	sf::RectangleShape m_Highlight;
};