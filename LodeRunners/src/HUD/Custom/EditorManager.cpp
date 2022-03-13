#include "EditorManager.h"

#include "../../Assets/Assets.h"

EditorManager::EditorManager(Ref<LevelAsset>& levelRef)
	: m_LevelRef(levelRef)
{
	m_QuitButton = MakeRef<TextButtonWidget>(this, "Quit");
	m_QuitButton->setRelativePosition({ .08f, 0.f });
	m_QuitButton->setRelativeSize({ .1f, 1.f });
	m_QuitButton->bindCallback([]() { Application::get()->killState(); });

	m_LevelSelector = MakeRef<LevelSelector>(this);
	m_LevelSelector->setRelativePosition({ .19f, 0.f });
	m_LevelSelector->setRelativeSize({ .3f, 1.f });

	m_LoadButton = MakeRef<TextButtonWidget>(this, "Load");
	m_LoadButton->setRelativePosition({ .50f, 0.f });
	m_LoadButton->setRelativeSize({ .1f, .49f });
	m_LoadButton->bindCallback(BIND_FN(loadLevel));

	m_SaveButton = MakeRef<TextButtonWidget>(this, "Save");
	m_SaveButton->setRelativePosition({ .50f, .51f });
	m_SaveButton->setRelativeSize({ .1f, .49f });
	m_SaveButton->bindCallback(BIND_FN(saveLevel));

	m_ClearButton = MakeRef<TextButtonWidget>(this, "Clear");
	m_ClearButton->setRelativePosition({ .61f, 0.f });
	m_ClearButton->setRelativeSize({ .1f, 1.f });
	m_ClearButton->bindCallback(BIND_FN(clearLevel));

	m_CreateLevelBox = MakeRef<TextBoxWidget>(this);
	m_CreateLevelBox->setText("Level name");
	m_CreateLevelBox->setRelativePosition({ .72f, 0.f });
	m_CreateLevelBox->setRelativeSize({ .2f, .49f });

	m_CreateButton = MakeRef<TextButtonWidget>(this, "Create");
	m_CreateButton->setRelativePosition({ .72f, .51f });
	m_CreateButton->setRelativeSize({ .2f, .49f });
	m_CreateButton->bindCallback(BIND_FN(createLevel));
}

void EditorManager::loadLevel()
{
	// For loading, we want to force reload, to take saves in account. If we just want to get a level without bothering with in between saves, we can let it to its false default value.
	m_LevelRef = MakeRef<LevelAsset>(*Assets::getLevelAsset(m_LevelSelector->getSelectedLevelName(), true));
	m_LevelSelector->setLoadedLevel();

	m_LevelSelector->setEdited(false);
}

void EditorManager::saveLevel()
{
	if (m_LevelRef)
	{
		AssetLoader::saveLevel(m_LevelRef);
		m_LevelSelector->setEdited(false);
	}
}

void EditorManager::clearLevel()
{
	if (m_LevelRef)
	{
		m_LevelRef->fill(TileType::Blank);
		onEdited();
	}
}

void EditorManager::createLevel()
{
	m_LevelRef = MakeRef<LevelAsset>();
	m_LevelRef->setName(m_CreateLevelBox->getText());
	m_LevelRef->fill(TileType::Blank);
	saveLevel();

	const auto& levels = AssetLoader::getAvailableLevels();
	for (size_t i = 0; i < levels.size(); i++)
	{
		if (m_LevelRef->getName() == levels[i].first)
			m_LevelSelector->setSelectedLevelIndex(i);
	}

	m_LevelSelector->setLoadedLevel();

	LOG_INFO("Created level \"" + m_CreateLevelBox->getText() + "\".");
}
