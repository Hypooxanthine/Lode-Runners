#include "Assets.h"

// Static variables declarations
Ref<SpriteAsset> Assets::m_SkinIcon = MakeRef<SpriteAsset>();
unsigned int Assets::m_ElementSize = 0;
std::unordered_map<TileType, Ref<SpriteAsset>> Assets::m_Tiles = std::unordered_map<TileType, Ref<SpriteAsset>>();
std::unordered_map<FlipbookType, Ref<FlipbookAsset>> Assets::m_Flipbooks = std::unordered_map<FlipbookType, Ref<FlipbookAsset>>();
Ref<LevelAsset> Assets::m_CachedLevel = MakeRef<LevelAsset>();
Ref<FontAsset> Assets::m_Font = MakeRef<FontAsset>();

void Assets::load(const std::string& name)
{
	AssetLoader::loadSkin(name, m_Tiles, m_ElementSize, m_Font, m_Flipbooks);
}

Ref<const LevelAsset> Assets::getLevelAsset(const std::string& name, const bool& forceReload)
{
	if (forceReload || m_CachedLevel->getName() != name)
		AssetLoader::loadLevel(name, m_CachedLevel);

	return m_CachedLevel;
}
