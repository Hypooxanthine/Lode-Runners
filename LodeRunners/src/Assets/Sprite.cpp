#include "Sprite.h"

Sprite::Sprite()
	: sf::Sprite()
{}

Sprite::Sprite(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y)
	: sf::Sprite()
{
	load(spriteSheet, size, x, y);
}

void Sprite::load(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y)
{
	setTexture(spriteSheet);
	setTextureRect(sf::IntRect(x * size, y * size, size, size));
}
