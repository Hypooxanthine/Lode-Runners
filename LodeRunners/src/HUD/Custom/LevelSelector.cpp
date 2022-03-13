#include "LevelSelector.h"

#include "../../Assets/Assets.h"

LevelSelector::LevelSelector()
	: LevelSelector(nullptr)
{}

LevelSelector::LevelSelector(Widget* parent)
	: Widget(parent)
{
	m_PreviousLevelButton = MakeRef<TextButtonWidget>(this, "<");
	m_PreviousLevelButton->setRelativePosition({ 0.f, 0.f });
	m_PreviousLevelButton->setRelativeSize({ .125f, 1.f });
	m_PreviousLevelButton->bindCallback(BIND_FN(previousLevel));

	m_NextLevelButton = MakeRef<TextButtonWidget>(this, ">");
	m_NextLevelButton->setRelativePosition({ .875f, 0.f });
	m_NextLevelButton->setRelativeSize({ .125f, 1.f });
	m_NextLevelButton->bindCallback(BIND_FN(nextLevel));

	m_LevelNameText = MakeRef<TextWidget>(this);
	if (!AssetLoader::getAvailableLevels().empty())
	{
		m_LevelNameText->setText(AssetLoader::getAvailableLevels()[0].first);
		m_SelectedLevel = 0;
	}
	m_LevelNameText->setRelativePosition({ .15f, 0.f });
	m_LevelNameText->setRelativeSize({ .7f, 1.f });

	updateStyle();
}

void LevelSelector::previousLevel()
{
	if (AssetLoader::getAvailableLevels().size() == 0) return;
	m_SelectedLevel = (m_SelectedLevel + AssetLoader::getAvailableLevels().size() - 1) % AssetLoader::getAvailableLevels().size();
	updateStyle();
}

void LevelSelector::nextLevel()
{
	if (AssetLoader::getAvailableLevels().size() == 0) return;
	m_SelectedLevel = (m_SelectedLevel + 1) % AssetLoader::getAvailableLevels().size();
	updateStyle();
}

void LevelSelector::updateStyle()
{
	m_LevelNameText->setText(AssetLoader::getAvailableLevels()[m_SelectedLevel].first);

	if (m_LoadedLevel.has_value() && m_SelectedLevel == m_LoadedLevel)
	{
		m_LevelNameText->setColor(m_Edited ? sf::Color::Color(255, 127, 0) : sf::Color::Green);
		m_LevelNameText->setBold();
	}
	else
	{
		m_LevelNameText->setColor(sf::Color::White);
		m_LevelNameText->setBold(false);
	}
}
