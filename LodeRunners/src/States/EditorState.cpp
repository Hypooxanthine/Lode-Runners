#include "EditorState.h"

#include "../HUD/ButtonWidget.h"
#include "../HUD/TextWidget.h"

/* EditorToolkit class. */

#define ADD_TOOLKIT_TILE(tile) \
m_Tiles.push_back(MakeRef<SpriteAsset>(*Assets::getTile(tile))); \
m_Tiles[m_Tiles.size() - 1]->setPosition((float)Assets::getElementSize() / 2.f, (float)Assets::getElementSize() / 2.f + (float)(m_Tiles.size() - 1) * (float)Assets::getElementSize() * (3.f / 2.f));

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, .9f, .9f };
const sf::FloatRect TOOLKIT_VIEWPORT = { .9f, 0.f, .1f, 1.f };
const sf::FloatRect UI_VIEWPORT = { 0.f, .9f, 0.9f, .1f };

EditorToolkit::EditorToolkit()
{
	//ADD_TOOLKIT_TILE(TileType::Blank); // Blank tile is right click. We could add it tho.
	ADD_TOOLKIT_TILE(TileType::Brick);
	ADD_TOOLKIT_TILE(TileType::Stone);
	ADD_TOOLKIT_TILE(TileType::Bridge);
	ADD_TOOLKIT_TILE(TileType::Ladder);
	ADD_TOOLKIT_TILE(TileType::Gold);
	ADD_TOOLKIT_TILE(TileType::PlayerStart);
	ADD_TOOLKIT_TILE(TileType::EnnemyStart);

	m_SelectedHighlight.setSize({ (float)Assets::getElementSize(), (float)Assets::getElementSize() });
	m_SelectedHighlight.setFillColor(sf::Color::Transparent);
	m_SelectedHighlight.setOutlineThickness(-(1.f / 16.f) * (float)Assets::getElementSize());
	m_SelectedHighlight.setOutlineColor(sf::Color::Red);

	m_HoveredHighlight.setSize({ (float)Assets::getElementSize(), (float)Assets::getElementSize() });
	m_HoveredHighlight.setFillColor(sf::Color::Transparent);
	m_HoveredHighlight.setOutlineThickness(-(1.f / 16.f) * (float)Assets::getElementSize());
	m_HoveredHighlight.setOutlineColor(sf::Color::Yellow);

	m_ToolkitView = sf::View(sf::FloatRect
	(
		0.f, 
		0.f, 
		(float)(2.f * Assets::getElementSize()), 
		(1.5f * m_Tiles.size() + .5f) * (float)Assets::getElementSize()
	));
	m_ToolkitView.setViewport(TOOLKIT_VIEWPORT);

	onResize();
	select(m_Tiles[0]);
}

void EditorToolkit::update(const float& dt)
{
	const sf::Vector2i mousePos = sf::Mouse::getPosition(*Application::get()->getWindow());
	const sf::IntRect viewPort = Application::get()->getWindow()->getViewport(m_ToolkitView);

	if (viewPort.contains(mousePos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			select(getHoveredTile());
		else
			hover(getHoveredTile());
	}
}

void EditorToolkit::render(Ref<sf::RenderWindow> window)
{
	window->setView(m_ToolkitView);
	for (const Ref<SpriteAsset>& t : m_Tiles)
		window->draw(*t);
	if (m_RenderHoveredHighlight)
		window->draw(m_HoveredHighlight);
	window->draw(m_SelectedHighlight);
}

void EditorToolkit::onResize()
{
	sf::FloatRect viewportRect = (sf::FloatRect)Application::get()->getWindow()->getViewport(m_ToolkitView);
	const sf::Vector2f viewportSize =
	{
		viewportRect.width,
		viewportRect.height
	};
	const sf::Vector2f toolkitSize =
	{
		2.f * (float)Assets::getElementSize(),
		(1.5f * m_Tiles.size() + .5f) * (float)Assets::getElementSize()
	};

	const float toolkitRatio = toolkitSize.x / toolkitSize.y;
	const float viewportRatio = viewportSize.x / viewportSize.y;

	if (toolkitRatio > viewportRatio)
	{
		const float heightCorrection = toolkitSize.x * viewportSize.y / viewportSize.x - toolkitSize.y;

		m_ToolkitView.setSize(toolkitSize.x, toolkitSize.y + heightCorrection);
	}
	else
	{
		const float widthCorrection = viewportSize.x * toolkitSize.y / viewportSize.y - toolkitSize.x;

		m_ToolkitView.setSize(toolkitSize.x + widthCorrection, toolkitSize.y);
	}
}

void EditorToolkit::select(Ref<SpriteAsset> tile)
{
	if (tile && tile != m_SelectedTile)
	{
		m_SelectedHighlight.setPosition(tile->getPosition());
		m_SelectedTile = tile;
	}
}

void EditorToolkit::hover(Ref<SpriteAsset> tile)
{
	if (!tile)
	{
		m_RenderHoveredHighlight = false;
		m_HoveredTile = nullptr;
		return;
	}
	// Hovering doesn't replace selecting, and we do not hover an already hovered tile.
	if (tile != m_SelectedTile && tile != m_HoveredTile)
	{
		m_RenderHoveredHighlight = true;
		m_HoveredHighlight.setPosition(tile->getPosition());
		m_HoveredTile = tile;
	}
}

Ref<SpriteAsset> EditorToolkit::getHoveredTile()
{
	const auto mousePos = sf::Mouse::getPosition(*Application::get()->getWindow());
	const auto mouseWorld = Application::get()->getWindow()->mapPixelToCoords(mousePos, m_ToolkitView);

	for (Ref<SpriteAsset>& t : m_Tiles)
	{
		if (t->getGlobalBounds().contains(mouseWorld))
			return t;
	}

	return nullptr;
}

/* EditorState class. */

EditorState::EditorState()
	: m_LevelView(sf::FloatRect
	(
		0.f,
		0.f,
		(float)Assets::getElementSize() * TILES_WIDTH,
		(float)Assets::getElementSize() * TILES_HEIGHT)
	),
	m_Toolkit(EditorToolkit()),
	m_UI([this](const std::string& name) { this->loadLevel(name); })
{
	// We need to edit the level, so we make a copy of it (LevelAsset is const because of code safety).
	m_LevelAsset = MakeRef<LevelAsset>();
	m_LevelAsset->fill(TileType::Blank);

	m_LevelView.setViewport(LEVEL_VIEWPORT);

	m_Highlight.setSize({ (float)Assets::getElementSize(), (float)Assets::getElementSize() });
	m_Highlight.setFillColor(sf::Color::Transparent);
	m_Highlight.setOutlineThickness(-(1.f / 16.f) * (float)Assets::getElementSize());
	m_Highlight.setOutlineColor(sf::Color::Green);

	onResize();
}

void EditorState::init()
{
	
}

void EditorState::update(const float& dt)
{
	if (!Application::get()->getWindow()->hasFocus())
		return;

	// Useful variables. Their names speak for themselves.
	const sf::Vector2i mousePos = sf::Mouse::getPosition(*Application::get()->getWindow());
	const sf::IntRect viewPort = Application::get()->getWindow()->getViewport(m_LevelView);
	const Ref<const sf::RenderWindow> window = Application::get()->getWindow();
	const sf::Vector2f mouseWorld = window->mapPixelToCoords(mousePos, m_LevelView); // World position pointed by mouse cursor.
	const sf::Vector2i hoveredTile =
	{
		(int)std::floor(mouseWorld.x / (float)(Assets::getElementSize())),
		(int)std::floor(mouseWorld.y / (float)(Assets::getElementSize()))
	};

	if (sf::IntRect(0, 0, TILES_WIDTH, TILES_HEIGHT).contains(hoveredTile))
	{

		m_RenderHighlighter = true;
		m_Highlight.setPosition(m_LevelAsset->at(hoveredTile.x, hoveredTile.y)->getPosition());

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			m_LevelAsset->changeSprite((sf::Vector2u)hoveredTile, m_Toolkit.getSelectedTile());
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			m_LevelAsset->changeSprite((sf::Vector2u)hoveredTile, Assets::getTile(TileType::Blank));
	}
	else
	{
		m_RenderHighlighter = false;
	}

	m_Toolkit.update(dt);
	m_UI.update(dt);
}

void EditorState::render(Ref<sf::RenderWindow> window)
{
	window->setView(m_LevelView);
	m_LevelAsset->render(window);
	if (m_RenderHighlighter)
		window->draw(m_Highlight);
	window->setView(window->getDefaultView());
	m_Toolkit.render(window);
	m_UI.render(window);
}

void EditorState::onResize()
{
	const sf::FloatRect viewportRect = (sf::FloatRect)Application::get()->getWindow()->getViewport(m_LevelView);
	const sf::Vector2f viewportSize = 
	{
		viewportRect.width, 
		viewportRect.height 
	};
	const sf::Vector2f levelSize = 
	{
		(float)(TILES_WIDTH * Assets::getElementSize()),
		(float)(TILES_HEIGHT * Assets::getElementSize())
	};

	const float levelRatio = levelSize.x / levelSize.y;
	const float viewportRatio = viewportSize.x / viewportSize.y;

	if (levelRatio > viewportRatio)
	{
		const float heightCorrection = levelSize.x * viewportSize.y / viewportSize.x - levelSize.y;

		m_LevelView.setSize(levelSize.x, levelSize.y + heightCorrection);
	}
	else
	{
		const float widthCorrection = viewportSize.x * levelSize.y / viewportSize.y - levelSize.x;

		m_LevelView.setSize(levelSize.x + widthCorrection, levelSize.y);
	}

	m_Toolkit.onResize();
	m_UI.onResize();
}

void EditorState::loadLevel(const std::string& name)
{
	LOG_TRACE(name);
	m_LevelAsset = MakeRef<LevelAsset>(*Assets::getLevelAsset(name));
}

/* EditorUI Class. */

EditorUI::EditorUI(std::function<void(const std::string&)> loadCallback)
	: m_LoadCallback(loadCallback)
{
	m_HUD = MakeRef<Widget>();

	m_HUD->setViewport(UI_VIEWPORT);
	m_HUD->fillParent();
	
	auto previousLevelButton = MakeRef<ButtonWidget>();
	previousLevelButton->setRelativePosition({ .05f, .1f });
	previousLevelButton->setRelativeSize({ .05f, .8f });
	previousLevelButton->bindCallback(BIND_FN(previousLevel));
	Widget::addChild(previousLevelButton, m_HUD);

	auto nextLevelButton = MakeRef<ButtonWidget>();
	nextLevelButton->setRelativePosition({ .5f, .1f });
	nextLevelButton->setRelativeSize({ .05f, .8f });
	nextLevelButton->bindCallback(BIND_FN(nextLevel));
	Widget::addChild(nextLevelButton, m_HUD);

	auto previousLevelText = MakeRef<TextWidget>();
	previousLevelText->fillParent();
	previousLevelText->setText("<");
	Widget::addChild(previousLevelText, previousLevelButton);

	auto nextLevelText = MakeRef<TextWidget>();
	nextLevelText->fillParent();
	nextLevelText->setText(">");
	Widget::addChild(nextLevelText, nextLevelButton);

	m_LevelSelector = MakeRef<TextWidget>();
	if (!AssetLoader::getAvailableLevels().empty())
	{
		m_LevelSelector->setText(AssetLoader::getAvailableLevels()[0].first);
		m_SelectedLevel = 0;
	}
	m_LevelSelector->setRelativePosition({ .11f, .1f });
	m_LevelSelector->setRelativeSize({ .38f, .8f });
	Widget::addChild(m_LevelSelector, m_HUD);

	auto loadButton = MakeRef<ButtonWidget>();
	loadButton->setRelativePosition({ .56f, .1f });
	loadButton->setRelativeSize({ .1f, .8f });
	loadButton->bindCallback([this]()
		{
			this->m_LoadCallback(AssetLoader::getAvailableLevels()[this->m_SelectedLevel].first);
		});
	Widget::addChild(loadButton, m_HUD);

	auto loadText = MakeRef<TextWidget>();
	loadText->fillParent();
	loadText->setText("Charger");
	Widget::addChild(loadText, loadButton);
}

void EditorUI::update(const float& dt)
{
	m_HUD->update(dt);
}

void EditorUI::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);
}

void EditorUI::onResize()
{
	m_HUD->onResize();
}

void EditorUI::previousLevel()
{
	m_SelectedLevel = (m_SelectedLevel + AssetLoader::getAvailableLevels().size() - 1) % AssetLoader::getAvailableLevels().size();
	m_LevelSelector->setText(AssetLoader::getAvailableLevels()[m_SelectedLevel].first);
}

void EditorUI::nextLevel()
{
	m_SelectedLevel = (m_SelectedLevel + 1) % AssetLoader::getAvailableLevels().size();
	m_LevelSelector->setText(AssetLoader::getAvailableLevels()[m_SelectedLevel].first);
}
