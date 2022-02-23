#pragma once

#include <array>

#include "../Core/Base.h"
#include "Sprite.h"

class LevelAsset
{
	friend class AssetLoader;
public:
	LevelAsset();

	inline const std::string& getName() const { return m_Name; }
	inline constexpr size_t getSize() const { return m_Data.size(); }
	inline const Ref<Sprite> operator[](const size_t& index) const { return m_Data[index]; }
	inline const Ref<Sprite> at(const size_t& x, const size_t& y) const { return m_Data[y * TILES_WIDTH + x]; }
	inline const Ref<Sprite> at(const sf::Vector2u& pos) const { return at(pos.x, pos.y); }

private:
	std::string m_Name;
	std::array<Ref<Sprite>, TILES_WIDTH* TILES_HEIGHT> m_Data;
};

