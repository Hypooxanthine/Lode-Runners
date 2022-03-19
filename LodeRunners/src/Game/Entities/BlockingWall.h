#pragma once

#include "Entity.h"

class ColliderComponent;

class BlockingWall : public Entity
{
public:
	BlockingWall();

	void setSize(const sf::Vector2f& size);

private:
	ColliderComponent* m_Collider;
};

