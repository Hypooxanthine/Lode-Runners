#pragma once

#define GOLD_POINTS 150

#include "Tile.h"

class SpriteComponent;
class ColliderComponent;

class GoldTile : public Tile
{
public:
	GoldTile();

	void pickUp();
	void hide();
	void show();

	const bool& wasTaken() const;

private:
	SpriteComponent* m_Sprite;
	ColliderComponent* m_Collider;

	bool m_PickedUp = false;
};

