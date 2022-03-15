#include "FlipbookComponent.h"

#include "../../Assets/Assets.h"

FlipbookComponent::FlipbookComponent()
	: FlipbookComponent(FlipbookType::None)
{
}

FlipbookComponent::FlipbookComponent(const FlipbookType& type)
	: Component()
{
	if (type == FlipbookType::None)
		m_FbAsset = nullptr;
	else
		m_FbAsset = MakeRef<FlipbookAsset>(*Assets::getFlipbook(type));

}

void FlipbookComponent::render(Ref<sf::RenderWindow> window)
{
	if(m_FbAsset)
		m_FbAsset->render(window);
}

void FlipbookComponent::update(const float& dt)
{
}

void FlipbookComponent::setType(const FlipbookType& type)
{
	m_FbAsset = MakeRef<FlipbookAsset>(*Assets::getFlipbook(type));
}
