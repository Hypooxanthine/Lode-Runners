#pragma once

#include "Tile.h"

class ColliderComponent;

class StoneTile : public Tile
{
public:
	StoneTile();

protected:
	ColliderComponent* m_Collider;
};

