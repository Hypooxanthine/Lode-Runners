#pragma once
#include "States.h"
#include "../Assets/Assets.h"
#include "../HUD/Widget.h"
#include "../HUD/TextWidget.h"

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

private: // Private attributes
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
	EditorUI(std::function<void(const std::string&)> loadCallback);

	void update(const float& dt);
	void render(Ref<sf::RenderWindow> window);
	void onResize();

private: // Private methods
	// Callbacks
	void previousLevel();
	void nextLevel();

private: // Private members
	Ref<Widget> m_HUD;
	Ref<TextWidget> m_LevelSelector;

	size_t m_SelectedLevel;

	std::function<void(const std::string&)> m_LoadCallback;

};

class EditorState : public State
{
public:
	EditorState();

	virtual void init() override;
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

	virtual void onResize() override;

	virtual inline const sf::Color& getClearColor() const override
	{
		static auto c = sf::Color::Color(20, 20, 20);
		return c;
	}

	void loadLevel(const std::string& name);

private:
	EditorToolkit m_Toolkit;
	EditorUI m_UI;

	Ref<LevelAsset> m_LevelAsset;

	sf::View m_LevelView;

	bool m_RenderHighlighter = false;
	sf::RectangleShape m_Highlight;
};