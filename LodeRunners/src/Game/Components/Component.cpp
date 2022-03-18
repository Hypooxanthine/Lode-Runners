#include "Component.h"

#include "../../Assets/Assets.h"

sf::Vector2f Component::getRelativePosition() const
{
	return m_RelativePosition / SPACE_UNIT;
}

void Component::setRelativePosition(const sf::Vector2f& pos)
{
	m_RelativePosition = pos * SPACE_UNIT;
}

sf::Vector2f Component::getWorldPosition() const
{
	ASSERT(getParent() != nullptr, "Component has invalid parent reference.");
	
	return getParent()->getPosition() + getRelativePosition();
}
