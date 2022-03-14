#pragma once

#include "Tile.h"

class ColliderComponent;

class BridgeTile : public Tile
{
public:
	BridgeTile();

protected:
	ColliderComponent* m_Collider;
};

