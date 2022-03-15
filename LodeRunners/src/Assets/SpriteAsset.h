#pragma once

#include "../Core/Base.h"

enum class TileType;

class SpriteAsset : public sf::Sprite
{
public:
	SpriteAsset();
	SpriteAsset(const SpriteAsset& other) = default; // This class doesn't store any heap allocated member. sf::Sprite copy constructor will do the job.
	SpriteAsset(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y, const TileType& type);

	// For tile sprites.
	void load(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int& y, const TileType& type);

	// No need to set any type for a flipbook sheet.
	void load(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int& y);

	inline const void setType(const TileType& type) { m_Type = type; }
	inline const TileType& getType() const { return m_Type; }

private:
	TileType m_Type;
};

