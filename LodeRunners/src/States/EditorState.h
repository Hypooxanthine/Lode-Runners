#pragma once
#include "States.h"
#include "../Assets/Assets.h"

class EditorToolkit
{
public:
	EditorToolkit();

	inline Ref<const SpriteAsset> getSelectedTile() const { return m_SelectedTile; }

	void update(const float& dt);
	void render(Ref<sf::RenderWindow> window);

	void onResize();

private: // Private methods
	void select(Ref<SpriteAsset> tile);
	void hover(Ref<SpriteAsset> tile);

	Ref<SpriteAsset> getHoveredTile();

private: // Private attributes
	sf::View m_ToolkitView;

	std::vector<Ref<SpriteAsset>> m_Tiles;
	
	Ref<SpriteAsset> m_HoveredTile = nullptr;
	sf::RectangleShape m_HoveredHighlight;
	bool m_RenderHoveredHighlight = false;

	Ref<SpriteAsset> m_SelectedTile = nullptr;
	sf::RectangleShape m_SelectedHighlight;
};

class LevelAsset;

class EditorState : public State
{
public:
	EditorState();

	virtual void init();
	virtual void update(const float& dt);
	virtual void render(Ref<sf::RenderWindow>& window);

	virtual void onResize();

private:
	Ref<LevelAsset> m_LevelAsset;
	EditorToolkit m_Toolkit;

	sf::View m_LevelView;

	bool m_RenderHighlighter = false;
	sf::RectangleShape m_Highlight;
};