#pragma once

#include "Tile.h"

class ColliderComponent;

class LadderTile : public Tile
{
public:
	LadderTile();

protected:
	ColliderComponent* m_Collider;
};

