#pragma once

#include "../Core/Base.h"
#include "AssetLoader.h"

class Assets
{
public: // Public methods
	Assets() = delete; // Static only class

	static void load(const std::string& name = "default");
	static const unsigned int& getElementSize() { return m_ElementSize; }
	static Ref<const SpriteAsset> getTile(const TileType& tile) { return (*m_Tiles)[tile]; }
	static Ref<const FlipbookAsset> getFlipbook(const FlipbookType& fb) { return (*m_Flipbooks)[fb]; }
	static Ref<const LevelAsset> getLevelAsset(const std::string& name, const bool& forceReload = false);
	static Ref<const FontAsset> getFontAsset() { return m_Font; }


private: // Private attributes
	// Loaded data
	static Ref<SpriteAsset> m_SkinIcon;
	static unsigned int m_ElementSize;
	static Ref<std::unordered_map<TileType, Ref<SpriteAsset>>> m_Tiles;
	static Ref<std::unordered_map<FlipbookType, Ref<FlipbookAsset>>> m_Flipbooks;
	static Ref<LevelAsset> m_CachedLevel; // We can't load every levels because it could be a huge amount of data. But we can't reload the same level if it's asked multiple times, so we cache the last used level.
	static Ref<FontAsset> m_Font;
};

