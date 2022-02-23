#pragma once

#include "../Core/Base.h"
#include "AssetLoader.h"

class Assets
{
public: // Public methods
	Assets() = delete;

	static void load(const std::string& name = "default");
	static inline const unsigned int& getElementSize() { return m_ElementSize; }
	static inline const Ref<Sprite> getTile(const TileType& tile) { return (*m_Tiles)[tile]; }
	static inline const Ref<Flipbook> getFlipbook(const FlipbookType& fb) { return (*m_Flipbooks)[fb]; }
	static const Ref<LevelAsset> getLevelAsset(const std::string& name);

private: // Private attributes
	// Loaded data
	static Ref<Sprite> m_SkinIcon;
	static unsigned int m_ElementSize;
	static Ref<std::unordered_map<TileType, Ref<Sprite>>> m_Tiles;
	static Ref<std::unordered_map<FlipbookType, Ref<Flipbook>>> m_Flipbooks;
	static Ref<LevelAsset> m_CachedLevel; // We can't load every levels because it could be a huge amount of data. But we can't reload the same level if it's asked multiple times, so we cache the last used level.
};

