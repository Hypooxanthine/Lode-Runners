#include "Level.h"
#include "Entities/Tiles/Tile.h"

Level::Level(Ref<LevelAsset> levelAsset)
{
	init(levelAsset.get());
}

void Level::init(const LevelAsset* levelAsset)
{
	for (size_t i = 0; i < levelAsset->getSize(); i++)
	{
		m_Tiles[i] = Tile::MakeTile(levelAsset->at(i)->getType());
	}
}
