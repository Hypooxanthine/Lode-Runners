#pragma once

#include "../Core/Base.h"

class SpriteAsset;
enum class FlipbookType;

class FlipbookAsset
{
public:
	FlipbookAsset();
	FlipbookAsset(const FlipbookAsset& other);

	void render(Ref<sf::RenderWindow> window, const size_t& frame, const sf::Vector2f& worldPosition);

	void setType(const FlipbookType& type) { m_Type = type; }
	void addFrame(Ref<SpriteAsset> sheet);

	size_t getFramesNumber() const { return m_Frames.size(); }

private:
	FlipbookType m_Type;

	std::vector<Ref<SpriteAsset>> m_Frames;
};

