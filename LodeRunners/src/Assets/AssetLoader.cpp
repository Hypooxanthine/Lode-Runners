#include "AssetLoader.h"

#include <fstream>

#include "Assets.h"

// Macros for filling connections between xml IDs and C++ enums
#define FILL_XML_TILE_NAMES(x) m_XMLTileNames[TileType::x]  = #x
#define FILL_XML_FLIPBOOKS_NAMES(x) m_XMLFlipbookNames[FlipbookType::x]  = #x

// Macros for corrupted data
#define CORRUPTED(condition) ASSERT(condition, ((std::string)"config.xml file corrupted. Aborting.").c_str())
#define CANT_FIND(condition, x) ASSERT(condition, ((std::string)"Couldn't find \"" + x + "\". Aborting.").c_str())

// Static members declarations
std::map<TileType, std::string> AssetLoader::m_XMLTileNames;
std::map<FlipbookType, std::string> AssetLoader::m_XMLFlipbookNames;
std::vector<std::string> AssetLoader::m_AvailableSkins;
std::vector<std::pair<std::string, std::string>> AssetLoader::m_AvailableLevels;
unsigned int AssetLoader::m_ElementSize = 0;

void AssetLoader::init()
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

	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	fillAvailableSkins(docHandle);
		LOG_INFO("Available skins :");
		#ifdef _DEBUG
		for (const std::string& s : m_AvailableSkins)
			LOG_TRACE(s);
		#endif

	fillAvailableLevels(docHandle);
		LOG_INFO("Available levels :");
		#ifdef _DEBUG
		for (const auto& p : m_AvailableLevels)
			LOG_TRACE(p.first);
		#endif
}

void AssetLoader::fillAvailableSkins(tinyxml2::XMLHandle& handle)
{
	auto skinsElement = getSkins(handle);

	auto skinElement = skinsElement->FirstChildElement();
	CORRUPTED(skinElement);

	m_AvailableSkins.clear();

	for (skinElement;
		skinElement != nullptr;
		skinElement = skinElement->NextSiblingElement())
	{
		const char* namePtr;
		CORRUPTED(skinElement->QueryStringAttribute("name", &namePtr) == tinyxml2::XMLError::XML_SUCCESS);

		m_AvailableSkins.push_back(namePtr);
	}
}

void AssetLoader::fillAvailableLevels(tinyxml2::XMLHandle& handle)
{
	auto levelsElement = getLevels(handle);

	auto levelElement = levelsElement->FirstChildElement();
	CORRUPTED(levelElement);

	std::string levelsPath = getLevelsPath(handle);
	m_AvailableLevels.clear();

	for (levelElement;
		levelElement != nullptr;
		levelElement = levelElement->NextSiblingElement())
	{
		const char* namePtr = nullptr;
		CORRUPTED(levelElement->QueryStringAttribute("name", &namePtr) == tinyxml2::XMLError::XML_SUCCESS);

		const char* fileNamePtr = nullptr;
		CORRUPTED(levelElement->QueryStringAttribute("fileName", &fileNamePtr) == tinyxml2::XMLError::XML_SUCCESS);

		m_AvailableLevels.push_back(std::pair<std::string, std::string>(namePtr, levelsPath + fileNamePtr));
	}
}

void AssetLoader::loadSkin(const std::string& skinName, Ref<std::unordered_map<TileType,Ref<Sprite>>> tiles, unsigned int& elementSize, Ref<std::unordered_map<FlipbookType,Ref<Flipbook>>> flipbooks)
{
	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	LoadSpritesheet(docHandle, skinName);
	loadTiles(*tiles, elementSize, docHandle, skinName);
	loadFlipbooks(docHandle, skinName);
}

void AssetLoader::LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name)
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
	CANT_FIND(getSpriteSheet().loadFromFile(getSpriteSheetPath(handle, name)), spritesheetFileName);
}

void AssetLoader::loadTiles(std::unordered_map<TileType,Ref<Sprite>>& tiles, unsigned int& elementSize, tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting requested skin layout
	auto skinElement = getSkin(handle, name);

	const char* layoutName = skinElement->Attribute("layout");
	CORRUPTED(layoutName);

	auto layoutElement = getLayout(handle, layoutName);

	// Getting element size
	CORRUPTED(layoutElement->QueryUnsignedAttribute("elementSize", &m_ElementSize) == tinyxml2::XMLError::XML_SUCCESS);
	elementSize = m_ElementSize;

	// Getting Tiles
	auto tilesElement = getTiles(handle);

	LOG_INFO("Loading tiles :");

	for (const std::pair<TileType, std::string>& p : m_XMLTileNames)
	{
		auto pos = getTilePosition(tilesElement, p.second);
		LOG_TRACE(p.second + " tile loaded.");
		tiles[p.first] = MakeRef<Sprite>(getSpriteSheet(), elementSize, pos.x, pos.y);
	}

	LOG_INFO("Tiles loaded.");
}

void AssetLoader::loadFlipbooks(tinyxml2::XMLHandle& handle, const std::string& name)
{
	LOG_INFO("Flipbooks loaded.");
}

void AssetLoader::loadLevel(const std::string& name, Ref<LevelAsset> level)
{
	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	std::string levelPath = getLevelPath(docHandle, name);

	std::ifstream file(levelPath);

	ASSERT(file.is_open(), "Couldn't load " + levelPath + ".");

	for (size_t i = 0;
		i < level->getSize();
		i++)
	{

		int tile;
		file >> tile;

		ASSERT(tile >= (int)TileType::Blank && tile <= (int)TileType::EnnemyStart, "Found invalid tile value in " + levelPath + ". Tile number : " + std::to_string(i + 1) + ", invalid value : " + std::to_string(tile) + ".");

		level->m_Data[i] = MakeRef<Sprite>(*Assets::getTile((TileType)tile));
		level->m_Data[i]->setPosition((i % TILES_WIDTH) * m_ElementSize, (int)(i / TILES_WIDTH) * m_ElementSize);
		
		// We're not supposed to reach the (TILES_HEIGHT*TILES_WIDTH+1)th value for i, so we never reach eofbit for a valid level save file.
		ASSERT(file.rdstate() != std::ios::eofbit, "Couldn't load " + name + " : insufficient number of tiles. Found " + std::to_string(i) + ".");
	}

	level->m_Name = name;

	file.close();
}

tinyxml2::XMLElement* AssetLoader::getSkins(tinyxml2::XMLHandle& handle)
{
	auto skinsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Skins").ToElement();
	CORRUPTED(skinsElement);
	return skinsElement;
}

tinyxml2::XMLElement* AssetLoader::getSkin(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting skins
	auto skinsElement = getSkins(handle);

	// Getting requested skin settings
	auto skinElement = skinsElement->FirstChildElement("Skin");
	CORRUPTED(skinElement);

	for (skinElement;
		skinElement != nullptr && skinElement->Attribute("name") != name;
		skinElement = skinElement->NextSiblingElement("Skin"));
	CANT_FIND(skinElement, name);

	return skinElement;
}

std::string AssetLoader::getSpriteSheetPath(tinyxml2::XMLHandle& handle, const std::string& name)
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
	return (std::string)spritesheetPath + spritesheetFileName;
}

tinyxml2::XMLElement* AssetLoader::getLayouts(tinyxml2::XMLHandle& handle)
{
	auto layoutsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").ToElement();
	CORRUPTED(layoutsElement);
	return layoutsElement;
}

tinyxml2::XMLElement* AssetLoader::getLayout(tinyxml2::XMLHandle& handle, const std::string& name)
{
	auto layoutElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").ToElement();
	CORRUPTED(layoutElement);

	for (layoutElement;
		layoutElement != nullptr && (std::string)layoutElement->Attribute("name") != name;
		layoutElement = layoutElement->NextSiblingElement("Layout"));
	CANT_FIND(layoutElement, name);

	return layoutElement;
}

tinyxml2::XMLElement* AssetLoader::getTiles(tinyxml2::XMLHandle& handle)
{
	auto tilesElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").FirstChildElement("Tiles").ToElement();
	CORRUPTED(tilesElement);

	return tilesElement;
}

AssetLoader::ElementPosition AssetLoader::getTilePosition(const tinyxml2::XMLElement* tilesElement, const std::string& name)
{
	using namespace tinyxml2;

	auto tileElement = tilesElement->FirstChildElement("Tile");
	CORRUPTED(tileElement);

	for (tileElement;
		tileElement != nullptr && std::string(tileElement->Attribute("name")) != name;
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

tinyxml2::XMLElement* AssetLoader::getLevels(tinyxml2::XMLHandle& handle)
{
	auto levelsElement = handle.FirstChildElement("App").FirstChildElement("Levels").ToElement();
	CORRUPTED(levelsElement);
	return levelsElement;
}

tinyxml2::XMLElement* AssetLoader::getLevel(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting levels
	auto levelsElement = getLevels(handle);

	// Getting requested skin settings
	auto levelElement = levelsElement->FirstChildElement("Level");
	CORRUPTED(levelElement);

	for (levelElement;
		levelElement != nullptr && levelElement->Attribute("name") != name;
		levelElement = levelElement->NextSiblingElement("Level"));
	CANT_FIND(levelElement, name);

	return levelElement;
}

std::string AssetLoader::getLevelsPath(tinyxml2::XMLHandle& handle)
{
	auto levelsPathElement = handle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("Levels").ToElement();
	CORRUPTED(levelsPathElement);
	const char* levelPath = nullptr;
	CORRUPTED(levelsPathElement->QueryStringAttribute("path", &levelPath) == tinyxml2::XMLError::XML_SUCCESS);

	return levelPath;
}

std::string AssetLoader::getLevelPath(tinyxml2::XMLHandle& handle, const std::string& name)
{
	for (const auto& p : m_AvailableLevels)
	{
		if (p.first == name)
			return p.second;
	}

	Application::get()->emergencyStop("Couldn't find " + name + " path.");
}

