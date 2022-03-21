#pragma once

#include "../Widget.h"

class TextWidget;

class GameUI : public Widget
{
public:
	GameUI(Widget* parent, const std::string& playerName, const std::string& levelName);

	void setScore(const size_t& score);

private:
	Ref<TextWidget> m_PlayerNameText;
	Ref<TextWidget> m_ScoreText;
	Ref<TextWidget> m_LevelText;
};

