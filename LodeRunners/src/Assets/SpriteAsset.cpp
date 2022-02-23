#include "SpriteAsset.h"

#include "AssetLoader.h"

SpriteAsset::SpriteAsset()
	: sf::Sprite(), m_Type(TileType::Blank)
{}

SpriteAsset::SpriteAsset(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y, const TileType& type)
	: sf::Sprite()
{
	load(spriteSheet, size, x, y, type);
}

void SpriteAsset::load(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y, const TileType& type)
{
	setTexture(spriteSheet);
	setTextureRect(sf::IntRect(x * size, y * size, size, size));
	m_Type = type;
}
