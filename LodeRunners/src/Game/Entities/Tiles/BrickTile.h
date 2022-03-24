#pragma once

#include "Tile.h"

class ColliderComponent;

class BrickTile : public Tile
{
public:
	BrickTile();

	void dig();

private: // Private member functions
	void hide();
	void show();

protected:
	ColliderComponent* m_Collider;

	bool m_IsDug = false;
};

