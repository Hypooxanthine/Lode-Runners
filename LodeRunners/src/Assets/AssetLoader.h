#pragma once

#include "../Core/Base.h"
#include "../Core/tinyxml2.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "LevelAsset.h"

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
};

enum class FlipbookType
{
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
};

class AssetLoader
{
public:
	AssetLoader() = delete;

	static void init();

	static inline const std::vector<std::string> getAvailableSkins() { return m_AvailableSkins; }
	static void loadSkin(const std::string& skinName, Ref<std::unordered_map<TileType,Ref<Sprite>>> tiles, unsigned int& elementSize, Ref<std::unordered_map<FlipbookType,Ref<Flipbook>>> flipbooks);

	static inline const std::vector<std::pair<std::string, std::string>> getAvailableLevels() { return m_AvailableLevels; }
	static void loadLevel(const std::string& name, Ref<LevelAsset> level);

private:
	struct ElementPosition { unsigned int x = 0, y = 0; };

private: // Private methods : interactions with config.xml
	// Highest level loadings
	static void LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadTiles(std::unordered_map<TileType,Ref<Sprite>>& tiles, unsigned int& elementSize, tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadFlipbooks(tinyxml2::XMLHandle& handle, const std::string& name);

	// Filling available assets lists
	static void fillAvailableSkins(tinyxml2::XMLHandle& handle);
	static void fillAvailableLevels(tinyxml2::XMLHandle& handle);

	// Skins
	static tinyxml2::XMLElement* getSkins(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getSkin(tinyxml2::XMLHandle& handle, const std::string& name);
	static std::string getSpriteSheetPath(tinyxml2::XMLHandle& handle, const std::string& name);
	static tinyxml2::XMLElement* getLayouts(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getLayout(tinyxml2::XMLHandle& handle, const std::string& name);

	// Tiles
	static tinyxml2::XMLElement* getTiles(tinyxml2::XMLHandle& handle);
	static ElementPosition getTilePosition(const tinyxml2::XMLElement* tilesElement, const std::string& name);

	// TODO : Flipbooks

	// TODO : Levels
	static tinyxml2::XMLElement* getLevels(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getLevel(tinyxml2::XMLHandle& handle, const std::string& name);
	static std::string getLevelsPath(tinyxml2::XMLHandle& handle);
	static std::string getLevelPath(tinyxml2::XMLHandle& handle, const std::string& name);

private: // Private members
	// Links between xml config file IDs/names and enums. Simple maps to be able to iterate through them.
	static std::map<TileType, std::string> m_XMLTileNames;
	static std::map<FlipbookType, std::string> m_XMLFlipbookNames;

	// Lists of available assets
	static std::vector<std::string> m_AvailableSkins;
	static std::vector<std::pair<std::string, std::string>> m_AvailableLevels; // first = name, second = path

	// Spritesheet is managed differently. It is treated like a static member, but sf::Texture objects need a valid OpenGL context to be created. We have to make it this way because we can't know when static member definitions are taken in account.
	// Spritesheet is stored in AssetLoader because Assets container class shouldn't access it.
	static sf::Texture& getSpriteSheet()
	{
		static sf::Texture t;
		return t;
	}

};

