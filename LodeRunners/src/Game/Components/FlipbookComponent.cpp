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
		m_FbAsset->render(window, m_CurrentFrame, getWorldPosition());
}

void FlipbookComponent::update(const float& dt)
{
}

void FlipbookComponent::setType(const FlipbookType& type)
{
	m_FbAsset = MakeRef<FlipbookAsset>(*Assets::getFlipbook(type));
}

void FlipbookComponent::setTotalDuration(const float& duration)
{
	setFrameDuration(duration / (float)m_FbAsset->getFramesNumber());
}

float FlipbookComponent::getTotalDuration() const
{
	return getFrameDuration() * (float)m_FbAsset->getFramesNumber();
}
