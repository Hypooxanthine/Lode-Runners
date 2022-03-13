#include "LevelSelector.h"

#include "../../Assets/Assets.h"

LevelSelector::LevelSelector()
	: LevelSelector(nullptr)
{}

LevelSelector::LevelSelector(Widget* parent)
	: Widget(parent)
{
	m_PreviousLevelButton = MakeRef<ButtonWidget>(this);
	m_PreviousLevelButton->setRelativePosition({ 0.f, 0.f });
	m_PreviousLevelButton->setRelativeSize({ .125f, 1.f });
	m_PreviousLevelButton->bindCallback(BIND_FN(previousLevel));

	m_NextLevelButton = MakeRef<ButtonWidget>(this);
	m_NextLevelButton->setRelativePosition({ .875f, 0.f });
	m_NextLevelButton->setRelativeSize({ .125f, 1.f });
	m_NextLevelButton->bindCallback(BIND_FN(nextLevel));

	m_PreviousLevelText = MakeRef<TextWidget>(m_PreviousLevelButton.get());
	m_PreviousLevelText->fillParent();
	m_PreviousLevelText->setText("<");

	m_NextLevelText = MakeRef<TextWidget>(m_NextLevelButton.get());
	m_NextLevelText->fillParent();
	m_NextLevelText->setText(">");

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
	m_LevelNameText->setText(AssetLoader::getAvailableLevels()[m_SelectedLevel].first);
	updateStyle();
}

void LevelSelector::nextLevel()
{
	if (AssetLoader::getAvailableLevels().size() == 0) return;
	m_SelectedLevel = (m_SelectedLevel + 1) % AssetLoader::getAvailableLevels().size();
	m_LevelNameText->setText(AssetLoader::getAvailableLevels()[m_SelectedLevel].first);
	updateStyle();
}

void LevelSelector::updateStyle()
{
	switch (m_Style)
	{
	case Style::Normal:
		m_LevelNameText->setColor(sf::Color::White);
		m_LevelNameText->setBold(false);
		break;
	case Style::Loaded:
		m_LevelNameText->setColor(sf::Color::Green);
		m_LevelNameText->setBold();
		break;
	case Style::Edited:
		m_LevelNameText->setColor(sf::Color::Color(255, 127, 0));
		m_LevelNameText->setBold();
		break;
	default:
		break;
	}
}
