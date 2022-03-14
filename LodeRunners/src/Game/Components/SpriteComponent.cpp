#include "SpriteComponent.h"

#include "../../Assets/Assets.h"

SpriteComponent::SpriteComponent(const SpriteAsset* asset)
{
	m_SpriteAsset = MakeRef<SpriteAsset>(*asset);
}

void SpriteComponent::render(Ref<sf::RenderWindow> window)
{
	window->draw(*m_SpriteAsset);
}

sf::Vector2f SpriteComponent::getWorldSize() const
{
	auto bounds = m_SpriteAsset->getGlobalBounds();
	return { bounds.width, bounds.height };
}
