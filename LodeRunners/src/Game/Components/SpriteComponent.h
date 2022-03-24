#pragma once

#include "Component.h"

class SpriteAsset;

class SpriteComponent : public Component
{
public:
	SpriteComponent(const SpriteAsset* asset);

	virtual void render(Ref<sf::RenderWindow> window) override;

	sf::Vector2f getWorldSize() const;

	void setSprite(const SpriteAsset* asset);

private:
	Ref<SpriteAsset> m_SpriteAsset;
};

