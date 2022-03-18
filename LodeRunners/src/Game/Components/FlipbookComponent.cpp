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
	m_TimeAccumulator += dt;

	if (m_TimeAccumulator > m_FrameDuration)
	{
		nextFrame();
		m_TimeAccumulator -= m_FrameDuration;
	}
}

const FlipbookType& FlipbookComponent::getType() const
{
	return m_FbAsset->getType();
}

void FlipbookComponent::setType(const FlipbookType& type)
{
	m_FbAsset = MakeRef<FlipbookAsset>(*Assets::getFlipbook(type));
}

size_t FlipbookComponent::getFramesNumber() const
{
	return m_FbAsset->getFramesNumber();
}

void FlipbookComponent::nextFrame()
{
	setCurrentFrame((m_CurrentFrame + 1) % getFramesNumber());
}

void FlipbookComponent::previousFrame()
{
	// *2 because "moduling" negative values leads to unexpected results.
	setCurrentFrame((m_CurrentFrame * 2 - 1) % getFramesNumber());
}

void FlipbookComponent::setTotalDuration(const float& duration)
{
	setFrameDuration(duration / (float)m_FbAsset->getFramesNumber());
}

float FlipbookComponent::getTotalDuration() const
{
	return getFrameDuration() * (float)m_FbAsset->getFramesNumber();
}
