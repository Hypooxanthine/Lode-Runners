#include "Assets.h"

// Static variables declarations
Ref<Sprite> Assets::m_SkinIcon = MakeRef<Sprite>();
unsigned int Assets::m_ElementSize = 0;
Ref<std::unordered_map<TileType, Ref<Sprite>>> Assets::m_Tiles = MakeRef<std::unordered_map<TileType, Ref<Sprite>>>();
Ref<std::unordered_map<FlipbookType, Ref<Flipbook>>> Assets::m_Flipbooks = MakeRef<std::unordered_map<FlipbookType, Ref<Flipbook>>>();
Ref<LevelAsset> Assets::m_CachedLevel = MakeRef<LevelAsset>();

void Assets::load(const std::string& name)
{

	AssetLoader::loadSkin(name, m_Tiles, m_ElementSize, m_Flipbooks);
}

Ref<LevelAsset> Assets::getLevelAsset(const std::string& name)
{
	if (m_CachedLevel->getName() != name)
		AssetLoader::loadLevel(name, m_CachedLevel);

	return m_CachedLevel;
}
