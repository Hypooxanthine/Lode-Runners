#include "LevelAsset.h"

LevelAsset::LevelAsset()
{
	for (size_t i = 0; i < m_Data.size(); i++)
	{
		m_Data[i] = MakeRef<SpriteAsset>();
	}
}

LevelAsset::LevelAsset(const LevelAsset& other)
{
	LOG_TRACE("Copy constructor called");
	for (size_t i = 0; i < other.getSize(); i++)
		m_Data[i] = MakeRef<SpriteAsset>(*other.m_Data[i]);
}

void LevelAsset::changeSprite(const size_t& index, Ref<const SpriteAsset> sprite)
{
	m_Data[index]->setTextureRect(sprite->getTextureRect());
	m_Data[index]->setType(sprite->getType());
}

void LevelAsset::changeSprite(const size_t& x, const size_t& y, Ref<const SpriteAsset> sprite)
{
	changeSprite(y * TILES_WIDTH + x, sprite);
}

void LevelAsset::changeSprite(const sf::Vector2u& pos, Ref<const SpriteAsset> sprite)
{
	changeSprite((size_t)pos.y * TILES_WIDTH + (size_t)pos.x, sprite);
}

void LevelAsset::render(const Ref<sf::RenderWindow>& window) const
{
	for (const Ref<const SpriteAsset>& sprite : m_Data)
	{
		window->draw(*sprite);
	}
}
