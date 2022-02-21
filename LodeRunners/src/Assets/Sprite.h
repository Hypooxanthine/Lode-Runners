#pragma once

#include "../Core/Base.h"

class Sprite : public sf::Sprite
{
public:
	Sprite();
	Sprite(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y);

	void load(const sf::Texture& spriteSheet, const unsigned int& size, const unsigned int& x, const unsigned int y);
};

