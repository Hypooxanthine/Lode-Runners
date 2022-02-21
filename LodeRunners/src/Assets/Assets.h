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

	static bool load(const std::string& name = "default");
	static inline const Sprite& getTile(const TileType& tile) { return m_Tiles[tile]; }
	static inline const Flipbook& getFlipbook(const FlipbookType& fb) { return m_Flipbooks[fb]; }

private:
	struct ElementPosition { unsigned int x = 0, y = 0; };

private: // Private methods
	static void setup();
	static bool LoadSpritesheet(const std::string& name);
	static bool loadTiles(const std::string& name);
	static bool loadFlipbooks(const std::string& name);
	static sf::Texture& getSpriteSheet()
	{
		static sf::Texture t;
		return t;
	}

private: // Private attributes
	// Links between xml config file IDs/names and enums
	static std::map<TileType, std::string> m_XMLTileNames;
	static std::map<FlipbookType, std::string> m_XMLFlipbookNames;

	// Loaded data
	static Sprite m_SkinIcon;
	static unsigned int m_ElementSize;
	static std::unordered_map<TileType, Sprite> m_Tiles;
	static std::unordered_map<FlipbookType, Flipbook> m_Flipbooks;
};

