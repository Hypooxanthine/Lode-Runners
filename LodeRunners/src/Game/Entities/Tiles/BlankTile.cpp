#include "BlankTile.h"

#include "../../Components/SpriteComponent.h"
#include "../../../Assets/Assets.h"

BlankTile::BlankTile()
{
	m_SpriteComponent = makeComponent<SpriteComponent>("Sprite", Assets::getTile(TileType::Blank).get());
}
