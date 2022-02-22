#pragma once

#include "../Core/Base.h"
#include "Sprite.h"
#include "Flipbook.h"

	enum class TileType
	{
		Blank = 0,
		Brick,
		Stone,
		Ladder,
		Bridge,
		Gold,

		PlayerStart = 100,
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
		EnnemyLeft = 100,
		EnnemyRight,
		EnnemyClimbLadder,
		EnnemyClimbBridge,
		EnnemyFall,
	};

class Assets
{
public: // Public methods
	Assets() = delete;

	static void load(const std::string& name = "default");
	static inline const Sprite& getTile(const TileType& tile) { return m_Tiles[tile]; }
	static inline const Flipbook& getFlipbook(const FlipbookType& fb) { return m_Flipbooks[fb]; }

private:
	struct ElementPosition { unsigned int x = 0, y = 0; };

private: // Private methods
	static void setup();
	static void LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadTiles(tinyxml2::XMLHandle& handle, const std::string& name);
	static void loadFlipbooks(tinyxml2::XMLHandle& handle, const std::string& name);

	// Interactions with config.xml
	static tinyxml2::XMLElement* getSkins(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getSkin(tinyxml2::XMLHandle& handle, const std::string& name);
	static std::string getSkinPath(tinyxml2::XMLHandle& handle, const std::string& name);

	static tinyxml2::XMLElement* getLayouts(tinyxml2::XMLHandle& handle);
	static tinyxml2::XMLElement* getLayout(tinyxml2::XMLHandle& handle, const std::string& name);

	static tinyxml2::XMLElement* getTiles(tinyxml2::XMLHandle& handle);
	static ElementPosition getTilePosition(const tinyxml2::XMLElement* tilesElement, const std::string& name);

private: // Private attributes
	// Links between xml config file IDs/names and enums. Simple maps to be able to iterate through them.
	static std::map<TileType, std::string> m_XMLTileNames;
	static std::map<FlipbookType, std::string> m_XMLFlipbookNames;

	// Loaded data
	static Sprite m_SkinIcon;
	static unsigned int m_ElementSize;
	static std::unordered_map<TileType, Sprite> m_Tiles;
	static std::unordered_map<FlipbookType, Flipbook> m_Flipbooks;
	// Spritesheet is managed differently. It is treated like a static member, but sf::Texture objects need a valid OpenGL context to be created. We have to make it this way because we can't know when static member definitions are taken in account.
	static sf::Texture& getSpriteSheet()
	{
		static sf::Texture t;
		return t;
	}
};

