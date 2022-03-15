#pragma once

#include "Tile.h"

class ColliderComponent;

class LadderTile : public Tile
{
public:
	LadderTile();
	LadderTile(const bool& isExit);

protected:
	ColliderComponent* m_Collider;

	bool m_IsExit = false;
};

