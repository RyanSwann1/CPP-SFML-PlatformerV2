#include "Map.h"
#include "Utilities.h"
#include "EntityManager.h"
#include "SharedContext.h"
#include "TextureManager.h"
#include "Rat.h"

#include <fstream>
#include <sstream>
#include <iostream>

Map::Map(SharedContext * sharedContext)
	: m_sharedContext(sharedContext),
	m_maxMapSize(30, 30),
	m_defaultFriction(0.02f, 0.1f),
	m_gravity(0.1f, 0.1f),
	m_tileSize(0),
	m_nextLevel(false),
	m_currentMapIndex(0)
{
	m_sharedContext->m_map = this;
	loadInMaps("Maps.txt");
	loadInTiles("Tiles.txt");
}

Map::~Map()
{
}

void Map::loadMap(const std::string & mapName)
{
	std::ifstream file;
	file.open(mapName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open: " << mapName << "\n";
		return;
	}
	//Acts as coordinates
	unsigned int x = 0;
	unsigned int y = 0;
	std::string line;
	//Cycle through the line
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		//unsigned int id = 0;
		for (const auto &i : line)
		{
			unsigned int id = 0;
			if (std::atoi(&i) > 0)
			{
				 id = std::atoi(&i) - 1;
				 //std::cout << "Tile ID: " << id << "\n";
				 //Find appropriate tile
				 auto iter = m_tileSet.find(id);
				 if (iter != m_tileSet.cend())
				 {
					 Tile* tile = new Tile;
					 
					 tile->m_tileInfo = new TileInfo(*iter->second);
					 tile->m_tileInfo->m_position = sf::Vector2i(x, y);
					 //HACK - Meaning that tile is deadly
					 //Will have to use XML at some point to correctly do this all
					 if (tile->m_tileInfo->m_ID == 7) 
					 {
						 tile->m_deadly = true;
					 }
					 else if (tile->m_tileInfo->m_ID == 8)
					 {
						 tile->m_warp = true;
					 }
					 if (!m_tileMap.emplace(std::make_pair(convertCoords(x, y), tile)).second)
					 {
						 std::cerr << "Duplicate tile in position: " << x << " " << y << "\n";
					 }
				 }
			}
			++x;
		}
		++y;
		x = 0;

		std::string type;
		keyStream >> type;
		if (type == "Background")
		{
			std::string textureName;
			keyStream >> textureName;
			m_currentBackgroundName = textureName;

			TextureManager* textureManager = m_sharedContext->m_textureManager;
			if (textureManager->requireResource(m_currentBackgroundName))
			{
				m_currentBackgroundImage.setTexture(*textureManager->getResource(m_currentBackgroundName));
			}

			m_currentBackgroundImage.setScale(3, 3);
			m_currentBackgroundImage.setPosition(0, 0);

		}
		else if (type == "Rat")
		{
			sf::Vector2f spawnPosition;
			keyStream >> spawnPosition.x >> spawnPosition.y;
			m_sharedContext->m_entityManager->add(new Rat(m_sharedContext), spawnPosition);
		}
	}
	file.close();
}

void Map::draw(sf::RenderWindow & window)
{
	if (m_currentBackgroundName != "")
	{
		window.draw(m_currentBackgroundImage);
	}
	//window.draw(*m_currentBackgroundImage);
	for (const auto &i : m_tileMap)
	{
		sf::Sprite* sprite = &i.second->m_tileInfo->m_sprite;
		sf::Vector2i* pos = &i.second->m_tileInfo->m_position;
		//sf::Sprite& sprite = i.second->m_tileInfo->m_sprite;
		sprite->setPosition(pos->x * Sheet::TILE_SIZE, pos->y * Sheet::TILE_SIZE);
		window.draw(*sprite);
	}
}

void Map::update()
{
	if (m_nextLevel)
	{
		m_nextLevel = false;
		
		if (m_currentMapIndex >= static_cast<int>(MapNames::Total)) {
			return;
		}
		++m_currentMapIndex;
		//Find the next map to load
		auto cIter = m_maps.find(m_mapNames[m_currentMapIndex - 1]);
		if (cIter != m_maps.cend())
		{
			unloadCurrentMap();
			loadMap(cIter->second);
		}
		else {
			std::cerr << "Unable to find: " << cIter->second << "\n";
		}
		std::cout << "CURRENT MAP INDEX: " << m_currentMapIndex << "\n";
	}
}

Tile * Map::getTile(const unsigned int x, const unsigned int y)
{
	auto iter = m_tileMap.find(convertCoords(x, y));
	if (iter != m_tileMap.cend())
	{
		return iter->second;
	}
	return nullptr;
}

void Map::unloadCurrentMap()
{
	m_sharedContext->m_entityManager->purgeEntities();
	m_tileMap.clear();
}

void Map::loadInTiles(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		if (line[0] == '|') {
			continue;
		}
		
		unsigned int id = 0;
		std::string name;
		keyStream >> id >> name;
		if (!m_tileSet.emplace(id, new TileInfo(m_sharedContext->m_textureManager, "TileSheet", id)).second)
		{
			std::cerr << "Duplicate tile detected." << id << "\n";
		}
		else {
			std::cout << "Added\n";
		}
	}
	file.close();
}

void Map::loadInMaps(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open()) {
		std::cerr << "Couldn't open: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string mapName;
		std::string mapLocation;
		keyStream >> mapName >> mapLocation;
		if (!m_maps.emplace(std::make_pair(mapName, mapLocation)).second) 
		{
			std::cerr << "Failed to load '" << mapName << "' '" << mapLocation << "' " << "\n";
			continue;
		}
	}
	file.close();
}

void Map::updateBackgroundPosition(const sf::Vector2f & playerPos)
{

}
