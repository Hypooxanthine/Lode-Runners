#include "Tile.h"
#include "Tiles.h"

Ref<Tile> Tile::MakeTile(const TileType& type)
{
    switch (type)
    {
    default:
    case TileType::Blank:
        return MakeRef<BlankTile>();
        break;
    case TileType::Brick:
        return MakeRef<BrickTile>();
        break;
    case TileType::Stone:
        return MakeRef<StoneTile>();
        break;
    case TileType::Bridge:
        return MakeRef<BridgeTile>();
        break;
    case TileType::Ladder:
        return MakeRef<LadderTile>();
        break;
    }
}
