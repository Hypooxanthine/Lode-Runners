#pragma once

#include "../Core/Base.h"

class SpriteAsset;
enum class FlipbookType;

class FlipbookAsset
{
public:
	FlipbookAsset();
	FlipbookAsset(const FlipbookAsset& other);

	void render(Ref<sf::RenderWindow> window);

	void setType(const FlipbookType& type) { m_Type = type; }
	void addFrame(Ref<SpriteAsset> sheet);

	size_t getFramesNumber() const { return m_Frames.size(); }
	const size_t& getCurrentFrame() const { return m_CurrentFrame; }
	void setCurrentFrame(const size_t& f) { m_CurrentFrame = f; }

private:
	FlipbookType m_Type;

	std::vector<Ref<SpriteAsset>> m_Frames;

	size_t m_CurrentFrame = 0;
};

