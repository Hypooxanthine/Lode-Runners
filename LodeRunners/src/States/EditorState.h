#pragma once
#include "States.h"
#include "../Level/Level.h"

class EditorState : public State
{
public:
	EditorState();

	virtual void init();
	virtual void update(const float& dt);
	virtual void render(Ref<sf::RenderWindow>& window);

private:
	Ref<LevelAsset> m_LevelAsset;
};

