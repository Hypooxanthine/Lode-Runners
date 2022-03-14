#include "Component.h"

sf::Vector2f Component::getWorldPosition() const
{
	ASSERT(getParent() != nullptr, "Component has invalid parent reference.");
	
	return getParent()->getPosition() + m_RelativePosition;
}
