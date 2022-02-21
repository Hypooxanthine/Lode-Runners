#include "Assets.h"
#include "../Core/Application.h"

// Macros for filling connections between xml IDs and C++ enums
#define FILL_XML_TILE_NAMES(x) m_XMLTileNames[TileType::x]  = #x
#define FILL_XML_FLIPBOOKS_NAMES(x) m_XMLFlipbookNames[FlipbookType::x]  = #x

// Macros for corrupted data
#define CORRUPTED \
{\
	LOG_ERROR((std::string)"config.xml file corrupted. Line : " + std::to_string(__LINE__));\
	return false;\
}
#define CANT_FIND(x) \
{\
	LOG_ERROR((std::string)"Couldn't find \"" + x + "\". Line : " + std::to_string(__LINE__));\
	return false;\
}

// Static variables declarations
std::map<TileType, std::string> Assets::m_XMLTileNames;
std::map<FlipbookType, std::string> Assets::m_XMLFlipbookNames;
Sprite Assets::m_SkinIcon;
unsigned int Assets::m_ElementSize = 0;
std::unordered_map<TileType, Sprite> Assets::m_Tiles;
std::unordered_map<FlipbookType, Flipbook> Assets::m_Flipbooks;

bool Assets::load(const std::string& name)
{
	setup();

	if (LoadSpritesheet(name) && loadTiles(name) && loadFlipbooks(name))
	{
		LOG_INFO("Assets loaded successfuly.");
		return true;
	}
	else
	{
		return false;
	}
}

void Assets::setup()
{
	FILL_XML_TILE_NAMES(Blank);
	FILL_XML_TILE_NAMES(Brick);
	FILL_XML_TILE_NAMES(Stone);
	FILL_XML_TILE_NAMES(Bridge);
	FILL_XML_TILE_NAMES(Gold);
	FILL_XML_TILE_NAMES(Ladder);
	
	FILL_XML_FLIPBOOKS_NAMES(PlayerLeft);
	FILL_XML_FLIPBOOKS_NAMES(PlayerRight);
	FILL_XML_FLIPBOOKS_NAMES(PlayerDigLeft);
	FILL_XML_FLIPBOOKS_NAMES(PlayerDigRight);
	FILL_XML_FLIPBOOKS_NAMES(PlayerClimbLadder);
	FILL_XML_FLIPBOOKS_NAMES(PlayerClimbBridge);
	FILL_XML_FLIPBOOKS_NAMES(PlayerFall);
	FILL_XML_FLIPBOOKS_NAMES(EnnemyLeft);
	FILL_XML_FLIPBOOKS_NAMES(EnnemyRight);
	FILL_XML_FLIPBOOKS_NAMES(EnnemyClimbLadder);
	FILL_XML_FLIPBOOKS_NAMES(EnnemyClimbBridge);
	FILL_XML_FLIPBOOKS_NAMES(EnnemyFall);
}

bool Assets::LoadSpritesheet(const std::string& name)
{
	using namespace tinyxml2;
	XMLDocument doc;
	bool loadOK = doc.LoadFile("config.xml") == XMLError::XML_SUCCESS;

	if (!loadOK)
	{
		LOG_ERROR("Couldn't open config.xml.");
		return false;
	}

	XMLHandle docHandle(&doc);

	// Getting spritesheet file path
	auto pathElement = docHandle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("Skins").ToElement();
	if (!pathElement) CORRUPTED

	const char* spritesheetPath = pathElement->Attribute("path");
	if (!spritesheetPath) CORRUPTED

	// Getting skins
	auto skinsElement = docHandle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Skins").ToElement();
	if (!skinsElement) CORRUPTED

	// Getting requested skin settings
	auto skinElement = skinsElement->FirstChildElement("Skin");
	for (skinElement; skinElement != NULL && skinElement->Attribute("name") != name; skinElement = skinsElement->NextSiblingElement("Skin"));
	if (!skinElement) CANT_FIND(name)

	// Getting spritesheet file name
	const char* spritesheetFileName = skinElement->Attribute("fileName");
	if (!spritesheetFileName) CORRUPTED

	// Loading spritesheet
	if (!getSpriteSheet().loadFromFile((std::string)spritesheetPath + spritesheetFileName)) CANT_FIND(spritesheetFileName)

	return true;
}

bool Assets::loadTiles(const std::string& name)
{
	using namespace tinyxml2;
	XMLDocument doc;
	bool loadOK = doc.LoadFile("config.xml") == XMLError::XML_SUCCESS;

	if (!loadOK)
	{
		LOG_ERROR("Couldn't open config.xml.");
		return false;
	}

	XMLHandle docHandle(&doc);

	// Getting skins
	auto skinsElement = docHandle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Skins").ToElement();
	if (!skinsElement) CORRUPTED

	// Getting requested skin settings
	auto skinElement = skinsElement->FirstChildElement("Skin");
	for (skinElement;
		skinElement != NULL && skinElement->Attribute("name") != name;
		skinElement = skinElement->NextSiblingElement("Skin"));
	if (!skinElement) CANT_FIND(name)

	const char* layoutName = skinElement->Attribute("layout");
	if (!layoutName) CORRUPTED

	// Getting layouts
	auto layoutsElement = docHandle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").ToElement();
	if (!layoutsElement) CORRUPTED

	// Getting layout with "layoutName" name
	auto layoutElement = docHandle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").ToElement();
	if(!layoutElement) CORRUPTED

	for (layoutElement;
		layoutElement != NULL && (std::string)layoutElement->Attribute("name") != layoutName;
		layoutElement = layoutElement->NextSiblingElement("Layout"));
	if (!layoutElement) CANT_FIND(layoutName)

	// Getting element size
	m_ElementSize = layoutElement->IntAttribute("elementSize");
	if (!m_ElementSize) CORRUPTED

	// Getting Tiles
	auto tilesElement = docHandle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").FirstChildElement("Tiles").ToElement();
	if (!tilesElement) CORRUPTED

	auto getTilePosition = [&](const std::string& name) -> ElementPosition
	{
		auto tileElement = tilesElement->FirstChildElement("Tile");

		for (tileElement;
			tileElement != NULL && std::string(tileElement->Attribute("name")) != name;
			tileElement = tileElement->NextSiblingElement("Tile"));

		if (!tileElement)
			Application::get()->emergencyStop((name + " tile not found in config.xml.").c_str());

		ElementPosition pos;
		if (tileElement->QueryUnsignedAttribute("x", &(pos.x)) == XMLError::XML_SUCCESS && tileElement->QueryAttribute("y", &(pos.y)) == XMLError::XML_SUCCESS)
		{
			return pos;
		}
		else
			Application::get()->emergencyStop("config.xml file corrupted.");
	};

	for(const std::pair<TileType, std::string>& p : m_XMLTileNames)
	{
		auto pos = getTilePosition(p.second);
		LOG_TRACE(p.second);
		m_Tiles[p.first] = Sprite(getSpriteSheet(), m_ElementSize, pos.x, pos.y);
	}

	LOG_INFO("Tiles loaded.");
	return true;
}

bool Assets::loadFlipbooks(const std::string& name)
{
	LOG_INFO("Flipbooks loaded.");
	return true;
}

