#pragma once

#include "../Entity.h"
#include "../../../Assets/Assets.h"

class SpriteComponent;

class Tile : public Entity
{
public:
	static Ref<Tile> MakeTile(const TileType& type);

	sf::Vector2u getTileMapPosition() const;

	const bool& isNavigable() const { return m_Navigable; }

protected:
	SpriteComponent* m_SpriteComponent = nullptr;

	bool m_Navigable = false;
};

