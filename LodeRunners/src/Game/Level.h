#pragma once

#include "../Core/Base.h"
#include "../Assets/LevelAsset.h"
#include "Entities/Tiles/Tile.h"

class Level
{
public:
	Level(Ref<LevelAsset> levelAsset);

private:
	std::array<Ref<Tile>, TILES_HEIGHT* TILES_WIDTH> m_Tiles;
};

