#pragma once

#include "../Entity.h"

class SpriteComponent;

class Tile : public Entity
{
protected:
	SpriteComponent* m_SpriteComponent = nullptr;
};

