#include "EditorState.h"
#include "../Assets/Assets.h"

EditorState::EditorState()
{
	m_LevelAsset = Assets::getLevelAsset("Level_1");
}

void EditorState::init()
{
}

void EditorState::update(const float& dt)
{
}

void EditorState::render(Ref<sf::RenderWindow>& window)
{
}
