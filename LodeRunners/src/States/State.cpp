#include "State.h"

const sf::Color& State::getClearColor() const
{
	static auto c = sf::Color::Color(20, 20, 40);
	return c;
}
