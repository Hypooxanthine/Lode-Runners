#include "Assets.h"

// Macros for filling connections between xml IDs and C++ enums
#define FILL_XML_TILE_NAMES(x) m_XMLTileNames[TileType::x]  = #x
#define FILL_XML_FLIPBOOKS_NAMES(x) m_XMLFlipbookNames[FlipbookType::x]  = #x

// Macros for corrupted data
#define CORRUPTED(condition) ASSERT(condition, ((std::string)"config.xml file corrupted. Line : " + std::to_string(__LINE__) + ". Aborting.").c_str())
#define CANT_FIND(condition, x) ASSERT(condition, ((std::string)"Couldn't find \"" + x + "\". Line : " + std::to_string(__LINE__) + ". Aborting.").c_str())

// Static variables declarations
std::map<TileType, std::string> Assets::m_XMLTileNames;
std::map<FlipbookType, std::string> Assets::m_XMLFlipbookNames;
Sprite Assets::m_SkinIcon;
unsigned int Assets::m_ElementSize = 0;
std::unordered_map<TileType, Sprite> Assets::m_Tiles;
std::unordered_map<FlipbookType, Flipbook> Assets::m_Flipbooks;

void Assets::load(const std::string& name)
{
	setup();

	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	LoadSpritesheet(docHandle, name);
	loadTiles(docHandle, name);
	loadFlipbooks(docHandle, name);
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

void Assets::LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting requested skin settings
	auto skinElement = getSkin(handle, name);

	// Getting spritesheet file path
	auto pathElement = handle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("Skins").ToElement();
	CORRUPTED(pathElement);

	const char* spritesheetPath;
	CORRUPTED(pathElement->QueryStringAttribute("path", &spritesheetPath) == tinyxml2::XMLError::XML_SUCCESS);

	// Getting spritesheet file name
	const char* spritesheetFileName = nullptr;
	CORRUPTED(skinElement->QueryStringAttribute("fileName", &spritesheetFileName) == tinyxml2::XMLError::XML_SUCCESS);

	// Loading spritesheet
	CANT_FIND(getSpriteSheet().loadFromFile((std::string)spritesheetPath + spritesheetFileName), spritesheetFileName);
}

void Assets::loadTiles(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting requested skin layout
	auto skinElement = getSkin(handle, name);

	const char* layoutName = skinElement->Attribute("layout");
	CORRUPTED(layoutName);

	auto layoutElement = getLayout(handle, layoutName);

	// Getting element size
	CORRUPTED(layoutElement->QueryUnsignedAttribute("elementSize", &m_ElementSize) == tinyxml2::XMLError::XML_SUCCESS);

	// Getting Tiles
	auto tilesElement = getTiles(handle);

	for(const std::pair<TileType, std::string>& p : m_XMLTileNames)
	{
		auto pos = getTilePosition(tilesElement, p.second);
		LOG_TRACE(p.second + " tile loaded.");
		m_Tiles[p.first] = Sprite(getSpriteSheet(), m_ElementSize, pos.x, pos.y);
	}

	LOG_INFO("Tiles loaded.");
}

void Assets::loadFlipbooks(tinyxml2::XMLHandle& handle, const std::string& name)
{
	LOG_INFO("Flipbooks loaded.");
}

tinyxml2::XMLElement* Assets::getSkins(tinyxml2::XMLHandle& handle)
{
	// Getting skins
	auto skinsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Skins").ToElement();
	CORRUPTED(skinsElement);
	return skinsElement;
}

tinyxml2::XMLElement* Assets::getSkin(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting skins
	auto skinsElement = getSkins(handle);

	// Getting requested skin settings
	auto skinElement = skinsElement->FirstChildElement("Skin");
	for (skinElement; skinElement != NULL && skinElement->Attribute("name") != name; skinElement = skinsElement->NextSiblingElement("Skin"));
	CANT_FIND(skinElement, name);

	return skinElement;
}

std::string Assets::getSkinPath(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting requested skin settings (it contains the file name of the spritesheet to use)
	auto skinElement = getSkin(handle, name);

	// Getting spritesheet file path
	auto pathElement = handle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("Skins").ToElement();
	CORRUPTED(pathElement);

	const char* spritesheetPath = pathElement->Attribute("path");
	CORRUPTED(spritesheetPath);

	// Getting spritesheet file name
	const char* spritesheetFileName = skinElement->Attribute("fileName");
	CORRUPTED(spritesheetFileName);

	// Loading spritesheet
	CANT_FIND(getSpriteSheet().loadFromFile((std::string)spritesheetPath + spritesheetFileName), spritesheetFileName);
}

tinyxml2::XMLElement* Assets::getLayouts(tinyxml2::XMLHandle& handle)
{
	auto layoutsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").ToElement();
	CORRUPTED(layoutsElement);
	return layoutsElement;
}

tinyxml2::XMLElement* Assets::getLayout(tinyxml2::XMLHandle& handle, const std::string& name)
{
	auto layoutElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").ToElement();
	CORRUPTED(layoutElement);

	for (layoutElement;
		layoutElement != NULL && (std::string)layoutElement->Attribute("name") != name;
		layoutElement = layoutElement->NextSiblingElement("Layout"));
	CANT_FIND(layoutElement, name);

	return layoutElement;
}

tinyxml2::XMLElement* Assets::getTiles(tinyxml2::XMLHandle& handle)
{
	auto tilesElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").FirstChildElement("Tiles").ToElement();
	CORRUPTED(tilesElement);

	return tilesElement;
}

Assets::ElementPosition Assets::getTilePosition(const tinyxml2::XMLElement* tilesElement, const std::string& name)
{
	using namespace tinyxml2;

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
}

