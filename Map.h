#pragma once

#include "TextureManager.h"
#include <SFML\Graphics.hpp>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <array>

//TILE SET TUTORIAL
//http://www.dreamincode.net/forums/topic/232396-c-tile-engine-from-scratch-part-4/

//Implement the fly-weight pattern
//So taht every tile can draw from the original one instead of having it so every tile stores its own texture.

//Flyweight pattern

enum class MapNames
{
	Map1 = 0,
	Map2,
	Total 
};

struct Sheet
{
	static const int TILE_SIZE = 32;
	static const unsigned int WIDTH = 256;
	static const unsigned int HEIGHT = 256;
};

//enum class Sheet{Tile_Size = 32, Sheet_Width = 256, Sheet_Height = 256, Num_Layers = 4};
struct SharedContext;
struct TileInfo
{
	TileInfo(TextureManager* textureManager, const std::string& name, const unsigned int ID)
		: m_ID(ID)

	{
		if (textureManager->requireResource(name))
		{
			m_sprite.setTexture(*textureManager->getResource(name));

			sf::IntRect tileBoundaries;

			tileBoundaries.left = m_ID % (Sheet::WIDTH / Sheet::TILE_SIZE) * Sheet::TILE_SIZE;
			tileBoundaries.top = m_ID / (Sheet::HEIGHT / Sheet::TILE_SIZE) * Sheet::TILE_SIZE;
			tileBoundaries.width = Sheet::TILE_SIZE;
			tileBoundaries.height = Sheet::TILE_SIZE;

			//	sf::IntRect tileBoundaries(m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			//		m_id / (Sheet::Sheet_Height / Sheet::Tile_Size) * Sheet::Tile_Size,
			//		Sheet::Tile_Size, Sheet::Tile_Size);
			//m_sprite.setTextureRect(tileBoundaries);

			m_sprite.setTextureRect(tileBoundaries);
			//m_sprite.setTexture(*textureManager->getResource(name));
			//m_sprite.setTex
		}
	}

	sf::Sprite m_sprite;
	sf::Vector2i m_position; //Possibly unneeded
	unsigned int m_ID;
};

struct Tile
{
	Tile() 
		: m_deadly(false),
		m_warp(false)
	{}

	TileInfo* m_tileInfo;
	bool m_deadly; //Harms entity
	bool m_warp; //Loads the next level
};

class Map
{
public:
	Map(SharedContext* sharedContext);
	~Map();

	void loadNextLevel() { m_nextLevel = true; }
	sf::Vector2u getMaxMapSize() const { return m_maxMapSize; }
	sf::Vector2f getDefaultFriction() const { return m_defaultFriction; }
	sf::Vector2f getDefaultGravity() const { return m_gravity; }
	std::unordered_map<unsigned int, Tile*> getTileMap() const { return m_tileMap; }
	Tile* getTile(const unsigned int x, const unsigned int y);
	int getTileSize() const { return m_tileSize; }

	
	void draw(sf::RenderWindow& window);
	void update();

private:
	std::unordered_map<unsigned int, TileInfo*> m_tileSet;
	std::unordered_map<unsigned int, Tile*> m_tileMap;
	std::unordered_map<std::string, std::string> m_maps;
	SharedContext* m_sharedContext;
	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_defaultFriction;
	sf::Vector2f m_gravity;
	std::string m_currentBackgroundName;
	sf::Sprite m_currentBackgroundImage;
	bool m_nextLevel;
	void unloadCurrentMap();
	int m_currentMapIndex;
	std::array<std::string, static_cast<size_t>(MapNames::Total)> m_mapNames{ "Map1", "Map2" };

	unsigned int convertCoords(const unsigned int x, const unsigned int y) const { return (x * m_maxMapSize.x) + y; }
	void loadMap(const std::string& mapName);
	void loadInTiles(const std::string& fileName);
	void loadInMaps(const std::string& fileName);
	void updateBackgroundPosition(const sf::Vector2f& playerPos);
	int m_tileSize;
};