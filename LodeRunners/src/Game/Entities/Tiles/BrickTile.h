#pragma once

#include "Tile.h"

class ColliderComponent;

class BrickTile : public Tile
{
public:
	BrickTile();

protected:
	ColliderComponent* m_Collider;
};

