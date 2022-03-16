#include "LevelAsset.h"

#include "Assets.h"

LevelAsset::LevelAsset()
{
	for (size_t i = 0; i < m_Data.size(); i++)
	{
		m_Data[i] = MakeRef<SpriteAsset>();
	}
}

LevelAsset::LevelAsset(const LevelAsset& other)
{
	// Only using operator= to initilize this.
	*this = other;
}

LevelAsset& LevelAsset::operator=(const LevelAsset& other)
{
	for (size_t i = 0; i < other.getSize(); i++)
		m_Data[i] = MakeRef<SpriteAsset>(*other.m_Data[i]);
	m_Name = other.getName();

	return *this;
}

std::vector<sf::Vector2u> LevelAsset::getEnnemiesStart() const
{
	std::vector<sf::Vector2u> out;

	for (size_t i = 0; i < m_Data.size(); i++)
	{
		auto& t = m_Data[i];

		if (t->getType() == TileType::EnnemyStart)
			out.push_back({ (int)i % Assets::getElementSize(), (int)i / Assets::getElementSize() });
	}

	return out;
}

sf::Vector2u LevelAsset::getPlayerStart() const
{
	std::optional<sf::Vector2u> out;

	for (size_t i = 0; i < m_Data.size(); i++)
	{
		auto& t = m_Data[i];

		if (t->getType() == TileType::PlayerStart)
			out = { (int)i % Assets::getElementSize(), (int)i / Assets::getElementSize() };
	}
	
	ASSERT(out.has_value(), "Couldn't find player spawn in level " + m_Name + ".");

	return out.value();
}

void LevelAsset::fill(const TileType& type)
{
	for (size_t i = 0; i < getSize(); i++)
	{
		m_Data[i] = MakeRef<SpriteAsset>(*Assets::getTile(type));
		m_Data[i]->setPosition
		(
			(float)(i % TILES_WIDTH) * Assets::getElementSize(),
			(float)(i / TILES_WIDTH) * Assets::getElementSize()
		);
	}
}

bool LevelAsset::changeSprite(const size_t& index, Ref<const SpriteAsset> sprite)
{
	if (sprite->getType() == m_Data[index]->getType()) return false;

	m_Data[index]->setTextureRect(sprite->getTextureRect());
	m_Data[index]->setType(sprite->getType());
	return true;
}

bool LevelAsset::changeSprite(const size_t& x, const size_t& y, Ref<const SpriteAsset> sprite)
{
	return changeSprite(y * TILES_WIDTH + x, sprite);
}

bool LevelAsset::changeSprite(const sf::Vector2u& pos, Ref<const SpriteAsset> sprite)
{
	return changeSprite((size_t)pos.y * TILES_WIDTH + (size_t)pos.x, sprite);
}

void LevelAsset::render(const Ref<sf::RenderWindow>& window) const
{
	for (const Ref<const SpriteAsset>& sprite : m_Data)
	{
		window->draw(*sprite);
	}
}
