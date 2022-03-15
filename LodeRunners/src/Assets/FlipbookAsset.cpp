#include "FlipbookAsset.h"

#include "AssetLoader.h"

FlipbookAsset::FlipbookAsset()
	: m_Type(FlipbookType::None)
{
}

FlipbookAsset::FlipbookAsset(const FlipbookAsset& other)
	: m_Type(other.m_Type)
{
	// Deep copy of SpriteAssets.
	for (auto& s : other.m_Frames)
		m_Frames.push_back(MakeRef<SpriteAsset>(*s));
}

void FlipbookAsset::render(Ref<sf::RenderWindow> window, const size_t& frame, const sf::Vector2f& worldPosition)
{
	m_Frames[frame]->setPosition(worldPosition);
	window->draw(*m_Frames[frame]);
}

void FlipbookAsset::addFrame(Ref<SpriteAsset> sheet)
{
	m_Frames.push_back(sheet);
}
