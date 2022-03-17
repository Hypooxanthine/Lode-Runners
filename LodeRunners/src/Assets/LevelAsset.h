#pragma once

#include <array>

#include "../Core/Base.h"
#include "SpriteAsset.h"

class LevelAsset
{
public:
	LevelAsset();
	LevelAsset(const LevelAsset& other); // Copy constructor has to be redefined

	LevelAsset& operator=(const LevelAsset& other);

	std::vector<sf::Vector2u> getEnnemiesStart() const;
	sf::Vector2u getPlayerStart() const;
	bool isValid() const;

	void fill(const TileType& type);

	inline const std::string& getName() const { return m_Name; }
	inline void setName(const std::string& name) { m_Name = name; }

	inline constexpr size_t getSize() const { return m_Data.size(); }

	inline Ref<const SpriteAsset> operator[](const size_t& index) const { return m_Data[index]; }
	inline Ref<const SpriteAsset> at(const size_t& index) const { return m_Data[index]; }
	inline Ref<const SpriteAsset> at(const size_t& x, const size_t& y) const { return m_Data[y * TILES_WIDTH + x]; }
	inline Ref<const SpriteAsset> at(const sf::Vector2u& pos) const { return m_Data[(size_t)pos.y * TILES_WIDTH + (size_t)pos.x]; }

	inline void setSprite(const size_t& index, Ref<SpriteAsset> sprite) { m_Data[index] = sprite; }
	inline void setSprite(const size_t& x, const size_t& y, Ref<SpriteAsset> sprite) { m_Data[y * TILES_WIDTH + x] = sprite; }
	inline void setSprite(const sf::Vector2u& pos, Ref<SpriteAsset> sprite) { m_Data[(size_t)pos.y * TILES_WIDTH + (size_t)pos.x] = sprite; }

	bool changeSprite(const size_t& index, Ref<const SpriteAsset> sprite);
	bool changeSprite(const size_t& x, const size_t& y, Ref<const SpriteAsset> sprite);
	bool changeSprite(const sf::Vector2u& pos, Ref<const SpriteAsset> sprite);

	void render(const Ref<sf::RenderWindow>& window) const;

private:
	std::string m_Name;
	std::array<Ref<SpriteAsset>, TILES_WIDTH * TILES_HEIGHT> m_Data;
};

