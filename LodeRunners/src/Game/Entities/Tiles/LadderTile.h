#pragma once

#include "Tile.h"

class ColliderComponent;

class LadderTile : public Tile
{
public:
	LadderTile();

	void setExit(const bool& val = true);
	const bool& isExit() const { return m_IsExit; }

protected:
	ColliderComponent* m_Collider;

	bool m_IsExit = false;
};

