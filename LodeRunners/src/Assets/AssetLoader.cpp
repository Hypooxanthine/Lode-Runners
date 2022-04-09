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
std::string AssetLoader::m_LeaderBoardPath;
unsigned int AssetLoader::m_ElementSize = 0;

void AssetLoader::init()
{
	FILL_XML_TILE_NAMES(Blank);
	FILL_XML_TILE_NAMES(Brick);
	FILL_XML_TILE_NAMES(Stone);
	FILL_XML_TILE_NAMES(Bridge);
	FILL_XML_TILE_NAMES(Gold);
	FILL_XML_TILE_NAMES(Ladder);
	FILL_XML_TILE_NAMES(PlayerStart);
	FILL_XML_TILE_NAMES(EnnemyStart);
	FILL_XML_TILE_NAMES(LevelEnd);

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

	fillLeaderBoardPath(docHandle);
		LOG_INFO("LeaderBoard path : {}", m_LeaderBoardPath);
}

void AssetLoader::fillAvailableSkins(tinyxml2::XMLHandle& handle)
{
	auto skinsElement = getSkinsElement(handle);

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
	m_AvailableLevels.clear();
	auto levelsElement = getLevelsElement(handle);

	auto levelElement = levelsElement->FirstChildElement();
	if(!levelElement) return; // If no level does exist, we just don't fill the levels list.

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

void AssetLoader::fillLeaderBoardPath(tinyxml2::XMLHandle& handle)
{
	auto leaderBoardElement = getLeaderBoardElement(handle);

	const char* pathPtr = nullptr;
	CORRUPTED(leaderBoardElement->QueryStringAttribute("path", &pathPtr) == tinyxml2::XMLError::XML_SUCCESS);

	m_LeaderBoardPath = pathPtr;
}

void AssetLoader::loadSkin(const std::string& skinName, std::unordered_map<TileType,Ref<SpriteAsset>>& tiles, unsigned int& elementSize, Ref<FontAsset> font, std::unordered_map<FlipbookType,Ref<FlipbookAsset>>& flipbooks)
{
	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	LoadSpritesheet(docHandle, skinName);
	loadTiles(tiles, elementSize, docHandle, skinName);
	loadFont(font, docHandle, skinName);
	loadFlipbooks(flipbooks, docHandle, skinName);
}

void AssetLoader::LoadSpritesheet(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting requested skin settings
	auto skinElement = getSkinElement(handle, name);

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

void AssetLoader::loadTiles(std::unordered_map<TileType,Ref<SpriteAsset>>& tiles, unsigned int& elementSize, tinyxml2::XMLHandle& handle, const std::string& skinName)
{
	// Getting requested skin layout
	auto skinElement = getSkinElement(handle, skinName);

	const char* layoutName = skinElement->Attribute("layout");
	CORRUPTED(layoutName);

	auto layoutElement = getLayoutElement(handle, layoutName);

	// Getting element size
	CORRUPTED(layoutElement->QueryUnsignedAttribute("elementSize", &m_ElementSize) == tinyxml2::XMLError::XML_SUCCESS);
	elementSize = m_ElementSize;

	// Getting Tiles
	auto tilesElement = getTiles(handle);

	LOG_INFO("Loading tiles : ");

	for (const std::pair<TileType, std::string>& p : m_XMLTileNames)
	{
		auto pos = getTilePosition(tilesElement, p.second);
		tiles[p.first] = MakeRef<SpriteAsset>(getSpriteSheet(), elementSize, pos.x, pos.y, p.first);

		LOG_TRACE(p.second + " tile loaded.");
	}

	LOG_INFO("Tiles loaded.");
}

void AssetLoader::loadFlipbooks(std::unordered_map<FlipbookType, Ref<FlipbookAsset>>& flipbooks, tinyxml2::XMLHandle& handle, const std::string& name)
{
	LOG_INFO("Loading flipbooks : ");

	for (const std::pair<FlipbookType, std::string>& p : m_XMLFlipbookNames)
	{
		auto flipbookAsset = MakeRef<FlipbookAsset>();
		flipbookAsset->setType(p.first);

		auto flipbookElement = getFlipbook(handle, p.second, name);

		// Getting all frames from flipbook
		auto frameElement = flipbookElement->FirstChildElement("Frame");
		CORRUPTED(flipbookElement);

		for (frameElement;
			frameElement != nullptr;
			frameElement = frameElement->NextSiblingElement("Frame"))
		{
			ElementPosition framePosition = getFramePosition(frameElement);

			auto frameSpriteAsset = MakeRef<SpriteAsset>();
			frameSpriteAsset->load(getSpriteSheet(), m_ElementSize, framePosition.x, framePosition.y);
			flipbookAsset->addFrame(frameSpriteAsset);
		}

		flipbooks[p.first] = flipbookAsset;

		LOG_TRACE(p.second + " flipbook loaded : " + std::to_string(flipbookAsset->getFramesNumber()) + " frames.");
	}

	LOG_INFO("Flipbooks loaded.");
}

void AssetLoader::loadFont(Ref<FontAsset> font, tinyxml2::XMLHandle& handle, const std::string& name)
{
	std::string path = getFontsPath(handle) + getFontFileName(handle, name);
	ASSERT(font->loadFromFile(path), "Couldn't load " + path + ".");
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

		ASSERT(tile >= (int)TileType::Blank && tile < (int)TileType::TILES_NUMBER, "Found invalid tile value in " + levelPath + ". Tile number : " + std::to_string(i + 1) + ", invalid value : " + std::to_string(tile) + ".");

		auto sprite = MakeRef<SpriteAsset>(*Assets::getTile((TileType)tile));
		sprite->setPosition
		(
			(float)((i % TILES_WIDTH) * m_ElementSize),
			(float)((unsigned int)(i / TILES_WIDTH) * m_ElementSize)
		);
		level->setSprite(i, sprite);
		
		// We're not supposed to reach the (TILES_HEIGHT*TILES_WIDTH+1)th value for i, so we never reach eofbit for a valid level save file.
		ASSERT(file.rdstate() != std::ios::eofbit, "Couldn't load " + name + " : insufficient number of tiles. Found " + std::to_string(i) + ".");
	}

	level->setName(name);

	file.close();

	LOG_INFO((std::string)"Loaded level \"" + name + "\".");
}

void AssetLoader::saveLevel(Ref<const LevelAsset> level)
{
	tinyxml2::XMLDocument doc;
	ASSERT(doc.LoadFile("config.xml") == tinyxml2::XMLError::XML_SUCCESS, "Couldn't open config.xml.");
	tinyxml2::XMLHandle docHandle(&doc);

	auto levelsElement = getLevelsElement(docHandle);
	auto levelElement = levelsElement->FirstChildElement("Level");
	for (levelElement;
		levelElement != nullptr && levelElement->Attribute("name") != level->getName();
		levelElement = levelElement->NextSiblingElement("Level"));

	if (levelElement)
	{
		const char* path = nullptr;
	}
	else
	{
		std::string fileName = level->getName();

		for (char& c : fileName)
			if (c == ' ') c = '_';
		fileName += ".txt";

		auto newLevelElement = doc.NewElement("Level");
		newLevelElement->SetAttribute("name", level->getName().c_str());
		newLevelElement->SetAttribute("fileName", fileName.c_str());
		levelsElement->InsertEndChild(newLevelElement);
		doc.SaveFile("config.xml");
	}

	fillAvailableLevels(docHandle);
	saveLevel(level, getLevelPath(docHandle, level->getName()));
}

void AssetLoader::loadLeaderBoard(Ref<LeaderBoardAsset> leaderBoard)
{
	LOG_INFO("Loading leaderboard.");
	leaderBoard->clear();

	std::ifstream ifs;
	ifs.open(m_LeaderBoardPath);

	if (ifs.is_open())
	{
		LOG_INFO("Leaderboard opened.");

		std::string line;

		while (std::getline(ifs, line))
		{
			std::string playerName;
			uint64_t playerScore = 0;

			for (const char& ch : line)
			{
				if (ch < '0' || ch > '9')
					playerName += ch;
				else
					break;
			}

			playerName = playerName.substr(0, playerName.size() - 1);
			std::istringstream iss(line.substr(playerName.size()));

			ASSERT(iss >> playerScore, "File " + m_LeaderBoardPath + " corrupted.");

			leaderBoard->set(playerName, playerScore);
			LOG_TRACE("Leaderboard entry added : player \"{}\", score {}.", playerName, playerScore);
		}

		ifs.close();

		return;
	}
	else // Leaderboard doesn't exist
	{
		// No entry to add to the leaderboard.
		LOG_INFO("Couldn't open leaderboard at path {}. Starting with an empty leaderboard.", m_LeaderBoardPath);
		return;
	}
}

void AssetLoader::saveLeaderBoard(Ref<const LeaderBoardAsset> leaderBoard)
{
	LOG_INFO("Saving leaderboard at path {}.", m_LeaderBoardPath);

	std::ofstream ofs;
	ofs.open(m_LeaderBoardPath, std::ofstream::out | std::ofstream::trunc);

	ASSERT(ofs.is_open(), "Couldn't open file " + m_LeaderBoardPath + ".");

	std::vector<std::pair<std::string, uint64_t>> sorted;

	for (const auto& entry : leaderBoard->getData())
		sorted.push_back(entry);

	std::sort(sorted.rbegin(), sorted.rend(),
		[](const std::pair<std::string, uint64_t>& left, const std::pair<std::string, uint64_t>& right)
		{
			return left.second < right.second;
		});

	for (auto& [playerName, playerScore] : sorted)
	{
		ofs << playerName << " " << playerScore << '\n';
	}

	ofs.close();

	LOG_INFO("Leaderboard saved at path {}.", m_LeaderBoardPath);
}

tinyxml2::XMLElement* AssetLoader::getSkinsElement(tinyxml2::XMLHandle& handle)
{
	auto skinsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Skins").ToElement();
	CORRUPTED(skinsElement);
	return skinsElement;
}

tinyxml2::XMLElement* AssetLoader::getSkinElement(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting skins
	auto skinsElement = getSkinsElement(handle);

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
	auto skinElement = getSkinElement(handle, name);

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

tinyxml2::XMLElement* AssetLoader::getLayoutsElement(tinyxml2::XMLHandle& handle)
{
	auto layoutsElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").ToElement();
	CORRUPTED(layoutsElement);
	return layoutsElement;
}

tinyxml2::XMLElement* AssetLoader::getLayoutElement(tinyxml2::XMLHandle& handle, const std::string& name)
{
	auto layoutElement = handle.FirstChildElement("App").FirstChildElement("Spritesheets").FirstChildElement("Layouts").FirstChildElement("Layout").ToElement();
	CORRUPTED(layoutElement);

	for (layoutElement;
		layoutElement != nullptr && (std::string)layoutElement->Attribute("name") != name;
		layoutElement = layoutElement->NextSiblingElement("Layout"));
	CANT_FIND(layoutElement, name);

	return layoutElement;
}

tinyxml2::XMLElement* AssetLoader::getLayoutElementFromSkinName(tinyxml2::XMLHandle& handle, const std::string& skinName)
{
	auto skinElement = getSkinElement(handle, skinName);

	const char* layoutName = nullptr;
	CORRUPTED(skinElement->QueryStringAttribute("layout", &layoutName) == tinyxml2::XMLError::XML_SUCCESS);

	return getLayoutElement(handle, layoutName);
}

tinyxml2::XMLElement* AssetLoader::getLeaderBoardElement(tinyxml2::XMLHandle& handle)
{
	auto leaderBoardElement = handle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("LeaderBoard").ToElement();
	CORRUPTED(leaderBoardElement);

	return leaderBoardElement;
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

std::string AssetLoader::getFontsPath(tinyxml2::XMLHandle& handle)
{
	auto fontsPathElement = handle.FirstChildElement("App").FirstChildElement("Paths").FirstChildElement("Fonts").ToElement();
	CORRUPTED(fontsPathElement);
	
	const char* fontsPath = nullptr;
	CORRUPTED(fontsPathElement->QueryStringAttribute("path", &fontsPath) == tinyxml2::XMLError::XML_SUCCESS);


	return fontsPath;
}

std::string AssetLoader::getFontFileName(tinyxml2::XMLHandle& handle, const std::string& name)
{
	auto skin = getSkinElement(handle, name);

	const char* fontName;
	CORRUPTED(skin->QueryStringAttribute("font", &fontName) == tinyxml2::XMLError::XML_SUCCESS);

	auto fontsElement = handle.FirstChildElement("App").FirstChildElement("Fonts").ToElement();
	CORRUPTED(fontsElement);

	auto fontElement = fontsElement->FirstChildElement("Font");
	CORRUPTED(fontElement);

	for (fontElement;
		fontElement != nullptr && fontElement->Attribute("name") != (std::string)fontName;
		fontsElement->NextSiblingElement("Font"));
	CANT_FIND(fontElement, fontName);

	const char* fontFileName = nullptr;
	CORRUPTED(fontElement->QueryStringAttribute("fileName", &fontFileName) == tinyxml2::XMLError::XML_SUCCESS);

	return fontFileName;
}

AssetLoader::ElementPosition AssetLoader::getFramePosition(const tinyxml2::XMLElement* frameElement)
{
	ElementPosition out;

	CORRUPTED(frameElement->QueryUnsignedAttribute("x", &out.x) == tinyxml2::XMLError::XML_SUCCESS);
	CORRUPTED(frameElement->QueryUnsignedAttribute("y", &out.y) == tinyxml2::XMLError::XML_SUCCESS);

	return out;
}

tinyxml2::XMLElement* AssetLoader::getFlipbook(tinyxml2::XMLHandle& handle, const std::string& name, const std::string& skinName)
{
	auto flipbooksElement = getFlipbooks(handle, skinName);

	auto flipbookElement = flipbooksElement->FirstChildElement("Flipbook");
	CORRUPTED(flipbookElement);

	for (flipbookElement;
		flipbookElement != nullptr && name != flipbookElement->Attribute("name");
		flipbookElement = flipbookElement->NextSiblingElement("Flipbook"));
	CANT_FIND(flipbookElement, name);

	return flipbookElement;
}

tinyxml2::XMLElement* AssetLoader::getFlipbooks(tinyxml2::XMLHandle& handle, const std::string& skinName)
{
	auto layoutElement = getLayoutElementFromSkinName(handle, skinName);

	auto flipbooksElement = layoutElement->FirstChildElement("Flipbooks");
	CORRUPTED(flipbooksElement);

	return flipbooksElement;
}

tinyxml2::XMLElement* AssetLoader::getLevelsElement(tinyxml2::XMLHandle& handle)
{
	auto levelsElement = handle.FirstChildElement("App").FirstChildElement("Levels").ToElement();
	CORRUPTED(levelsElement);
	return levelsElement;
}

tinyxml2::XMLElement* AssetLoader::getLevel(tinyxml2::XMLHandle& handle, const std::string& name)
{
	// Getting levels
	auto levelsElement = getLevelsElement(handle);

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

	ASSERT(false, "Couldn't find " + name + " path.");
}

void AssetLoader::saveLevel(Ref<const LevelAsset> level, const std::string& path)
{
	std::ofstream ofs;
	ofs.open(path, std::ofstream::out | std::ofstream::trunc);

	ASSERT(ofs.is_open(), "Couldn't open file " + path + ".");

	for (size_t i = 0; i < level->getSize(); i++)
	{
		ofs << (int)level->at(i)->getType() << " ";
	}

	ofs.close();
}

