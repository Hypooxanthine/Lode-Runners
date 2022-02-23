#include "EditorState.h"
#include "../Assets/Assets.h"

EditorState::EditorState()
	: m_LevelView(sf::FloatRect(0.f, 0.f, (float)Assets::getElementSize() * TILES_WIDTH, (float)Assets::getElementSize() * TILES_HEIGHT)),
	m_LevelViewport(0.f, 0.f, 0.f, 0.f)
{
	// Unlike in a game, we need to edit the level in the editor. So we make an editable copy of it.
	m_LevelAsset = MakeRef<LevelAsset>(*Assets::getLevelAsset("Level_1"));

	onResize(Application::get()->getWindow()->getSize());

	m_Highlight.setSize({ (float)Assets::getElementSize(), (float)Assets::getElementSize() });
	m_Highlight.setFillColor(sf::Color::Transparent);
	m_Highlight.setOutlineThickness(-(1.f / 16.f) * (float)Assets::getElementSize());
	m_Highlight.setOutlineColor(sf::Color::Green);
}

void EditorState::init()
{

}

void EditorState::update(const float& dt)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*Application::get()->getWindow());
	sf::IntRect viewPort = Application::get()->getWindow()->getViewport(m_LevelView);

	sf::Vector2i hoveredTile = { (int)std::floor((float)(mousePos.x - viewPort.left) / (float)viewPort.width * (float)TILES_WIDTH),
		(int)std::floor((float)(mousePos.y - viewPort.top) / (float)viewPort.height * (float)TILES_HEIGHT) };

	if (hoveredTile.x >= 0 && hoveredTile.x < TILES_WIDTH && hoveredTile.y >= 0 && hoveredTile.y < TILES_HEIGHT)
	{
		m_RenderHighlighter = true;
		m_Highlight.setPosition(m_LevelAsset->at(hoveredTile.x, hoveredTile.y)->getPosition());

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			m_LevelAsset->changeSprite((sf::Vector2u)hoveredTile, Assets::getTile(TileType::Brick));
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			m_LevelAsset->changeSprite((sf::Vector2u)hoveredTile, Assets::getTile(TileType::Blank));
	}
	else
	{
		m_RenderHighlighter = false;
	}
}

void EditorState::render(Ref<sf::RenderWindow>& window)
{
	window->setView(m_LevelView);
	m_LevelAsset->render(window);
	if (m_RenderHighlighter)
		window->draw(m_Highlight);
	window->setView(window->getDefaultView());
}

void EditorState::onResize(const sf::Vector2u& size)
{
	// Some maths
	if ((9.f / 10.f) * ((float)TILES_HEIGHT / (float)TILES_WIDTH) <= (float)size.y / (float)size.x)
	{
		m_LevelViewport.width = (9.f / 10.f);
		m_LevelViewport.height = ((float)TILES_HEIGHT / (float)TILES_WIDTH) * (9.f / 10.f) * (float)size.x / (float)size.y;
		m_LevelViewport.top = (1.f - m_LevelViewport.height) / 2.f;
		m_LevelViewport.left = 0.f;
	}
	else
	{
		float widthBlank = (9.f / 10.f) - ((float)TILES_WIDTH / (float)TILES_HEIGHT) * ((float)size.y / (float)size.x);
		m_LevelViewport.width = (9.f / 10.f) - widthBlank;
		m_LevelViewport.height = 1.f;
		m_LevelViewport.left = widthBlank / 2.f;
		m_LevelViewport.top = 0.f;
	}

	m_LevelView.setViewport(m_LevelViewport);
}
