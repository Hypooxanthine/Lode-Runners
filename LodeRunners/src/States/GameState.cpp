#include "GameState.h"

#include "../Assets/Assets.h"

const sf::FloatRect LEVEL_VIEWPORT = { 0.f, 0.f, 1.f, 1.f };

GameState::GameState(Ref<LevelAsset> level)
	: m_Level(level), m_LevelView(sf::FloatRect
	(
		0.f,
		0.f,
		(float)Assets::getElementSize() * TILES_WIDTH,
		(float)Assets::getElementSize() * TILES_HEIGHT)
	)
{
	m_LevelView.setViewport(LEVEL_VIEWPORT);
}

void GameState::update(const float& dt)
{
}

void GameState::render(Ref<sf::RenderWindow> window)
{
	window->setView(m_LevelView);
	m_Level->render(window);
	window->setView(window->getDefaultView());
}

void GameState::onResize()
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
}
