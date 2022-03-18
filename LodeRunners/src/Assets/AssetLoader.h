#pragma once

#include "../Core/Base.h"
#include "../Core/tinyxml2.h"
#include "SpriteAsset.h"
#include "FlipbookAsset.h"
#include "LevelAsset.h"
#include "FontAsset.h"

enum class TileType
{
	Blank = 0,
	Brick,
	Stone,
	Ladder,
	Bridge,
	Gold,

	PlayerStart,
	EnnemyStart,
	LevelEnd,

	TILES_NUMBER
};

enum class FlipbookType
{
	None = -1,

	PlayerLeft = 0,
	PlayerRight,
	PlayerDigLeft,
	PlayerDigRight,
	PlayerClimbLadder,
	PlayerClimbBridge,
	PlayerFall,

	EnnemyLeft,
	EnnemyRight,
	EnnemyClimbLadder,
	EnnemyClimbBridge,
	EnnemyFall,

	FLIPBOOKS_NUMBER
};

class AssetLoader
{
public:
	AssetLoader() = delete;

	static void init();

	static inline const std::vector<std::string> getAvailableSkins() { return m_AvailableSkins; }
	static void loadSkin(const std::string& skinName, std::unordered_map<TileType,Ref<SpriteAsset>>& tiles, unsigned int& elementSize, Ref<FontAsset> font, std::unordered_map<FlipbookType,Ref<FlipbookAsset>>& flipbooks);

	static inline const std::vector<std::pair<std::string, std::string>>& getAvailableLevels() { return m_AvailableLevels; }
	static void loadLevel(const std::string& name, Ref<LevelAsset> level);
	static void saveLevel(Ref<const LevelAsset> level);

private:
	struct ElementPosition { unsigned int x = 0, y = 0; };

private: // Private member functions : interactions with config.xml
	// Highest level loadings
	static void LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadTiles(std::unordered_map<TileType,Ref<SpriteAsset>>& tiles, unsigned int& elementSize, tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadFlipbooks(std::unordered_map<FlipbookType, Ref<FlipbookAsset>>& flipbooks, tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadFont(Ref<FontAsset> font, tinyxml2::XMLHandle& handle, const std::string& name);

	// Filling available assets lists
	static void fillAvailableSkins(tinyxml2::XMLHandle& handle);
	static void fillAvailableLevels(tinyxml2::XMLHandle& handle);

	// Skins
	static tinyxml2::XMLElement* getSkins(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getSkin(tinyxml2::XMLHandle& handle, const std::string& skinName);
	static std::string getSpriteSheetPath(tinyxml2::XMLHandle& handle, const std::string& skinName);
	static tinyxml2::XMLElement* getLayouts(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getLayout(tinyxml2::XMLHandle& handle, const std::string& layoutName);
	static tinyxml2::XMLElement* getLayoutFromSkinName(tinyxml2::XMLHandle& handle, const std::string& skinName);

	// Tiles
	static tinyxml2::XMLElement* getTiles(tinyxml2::XMLHandle& handle);
	static ElementPosition getTilePosition(const tinyxml2::XMLElement* tilesElement, const std::string& name);

	// Font
	static std::string getFontsPath(tinyxml2::XMLHandle& handle);
	static std::string getFontFileName(tinyxml2::XMLHandle& handle, const std::string& name);

	// TODO : Flipbooks
	static ElementPosition getFramePosition(const tinyxml2::XMLElement* frameElement);
	static tinyxml2::XMLElement* getFlipbook(tinyxml2::XMLHandle& handle, const std::string& name, const std::string& skinName);
	static tinyxml2::XMLElement* getFlipbooks(tinyxml2::XMLHandle& handle, const std::string& skinName);

	// Levels
	static tinyxml2::XMLElement* getLevels(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getLevel(tinyxml2::XMLHandle& handle, const std::string& name);
	static std::string getLevelsPath(tinyxml2::XMLHandle& handle);
	static std::string getLevelPath(tinyxml2::XMLHandle& handle, const std::string& name);
	static void saveLevel(Ref<const LevelAsset> level, const std::string& path);

private: // Private members
	// Links between xml config file IDs/names and enums. Simple maps to be able to iterate through them.
	static std::map<TileType, std::string> m_XMLTileNames;
	static std::map<FlipbookType, std::string> m_XMLFlipbookNames;

	// Lists of available assets
	static std::vector<std::string> m_AvailableSkins;
	static std::vector<std::pair<std::string, std::string>> m_AvailableLevels; // first = name, second = path

	// Element size. Needed to load a level and to set tiles positions.
	static unsigned int m_ElementSize;

	// Spritesheet is managed differently. It is treated like a static member, but sf::Texture objects need a valid OpenGL context to be created. We have to make it this way because we can't know when static member definitions are taken in account.
	// Spritesheet is stored in AssetLoader because Assets container class shouldn't access it.
	static sf::Texture& getSpriteSheet()
	{
		static sf::Texture t;
		return t;
	}

};

