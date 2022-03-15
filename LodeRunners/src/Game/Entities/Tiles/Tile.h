#pragma once

#include "../Entity.h"
#include "../../../Assets/Assets.h"

class SpriteComponent;

class Tile : public Entity
{
public:
	static Ref<Tile> MakeTile(const TileType& type);

protected:
	SpriteComponent* m_SpriteComponent = nullptr;
};

