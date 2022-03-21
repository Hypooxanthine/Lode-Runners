#include "GameUI.h"

#include "../TextWidget.h"

GameUI::GameUI(Widget* parent, const std::string& playerName, const std::string& levelName)
	: Widget(parent)
{
	m_PlayerNameText = MakeRef<TextWidget>(this);
	m_PlayerNameText->setRelativePosition({ 0.f, 0.f });
	m_PlayerNameText->setRelativeSize({ 1.f / 3.f, 1.f });
	m_PlayerNameText->setText("Player: " + playerName);

	m_ScoreText = MakeRef<TextWidget>(this);
	m_ScoreText->setRelativePosition({ 1.f / 3.f, 0.f });
	m_ScoreText->setRelativeSize({ 1.f / 3.f, 1.f });
	setScore(0);

	m_LevelText = MakeRef<TextWidget>(this);
	m_LevelText->setRelativePosition({ 2.f / 3.f, 0.f });
	m_LevelText->setRelativeSize({ 1.f / 3.f, 1.f });
	m_LevelText->setText("Level: " + levelName);
}

void GameUI::setScore(const size_t& score)
{
	m_ScoreText->setText("Score: " + std::to_string(score));
}
