#include "Tile.h"
#include "Tiles.h"

Ref<Tile> Tile::MakeTile(const TileType& type)
{
    static size_t brickTilesID = 0;

    switch (type)
    {
    default:
    case TileType::Blank:
        return MakeRef<BlankTile>();
        break;
    case TileType::Brick:
        return MakeRef<BrickTile>(brickTilesID++);
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
    case TileType::PlayerStart:
        return MakeRef<PlayerSpawnTile>();
        break;
    case TileType::EnnemyStart:
        return MakeRef<EnnemySpawnTile>();
        break;
    case TileType::LevelEnd:
        return MakeRef<ExitTile>();
        break;
    }
}

sf::Vector2u Tile::getTileMapPosition() const
{
    return (sf::Vector2u)getPosition();
}
