#pragma once
#include "States.h"

class LevelAsset;

class EditorState : public State
{
public:
	EditorState();

	virtual void init();
	virtual void update(const float& dt);
	virtual void render(Ref<sf::RenderWindow>& window);

	virtual void onResize(const sf::Vector2u& size);

private:
	Ref<LevelAsset> m_LevelAsset;

	sf::View m_LevelView;
	sf::FloatRect m_LevelViewport;

	bool m_RenderHighlighter = false;
	sf::RectangleShape m_Highlight;
};

