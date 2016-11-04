#include "Computer.h"
#include "SharedContext.h"
#include "Map.h"
#include <iostream>
Computer::Computer(SharedContext* sharedContext)
	: Character(sharedContext)
{
}


Computer::~Computer()
{
}

void Computer::update(const float deltaTime)
{
	//check tile from map
	//have game go from one side to otehr
	//Camera follows
}

//void Computer::resolveCollisions()
//{
//	//Map* map = m_sharedContext->m_map;
//	//for (auto &i : m_collisions)
//	//{
//	//	sf::FloatRect intersection = i->m_intersection;
//	//	sf::FloatRect tileBoundary = i->m_collisionBox;
//	//	int tileSize = m_spriteSheet.getTileSize();
//	//	int y = std::floor(m_AABB.top / tileSize);
//	//	int fromX = 0;
//	//	int toX = 0;
//	//	//Distance from entity to check tile
//	//	if (m_currentDirection == Direction::Right) 
//	//	{
//	//		fromX = std::floor((m_AABB.left + m_AABB.width) / tileSize);
//	//		toX = std::floor((m_AABB.left + (m_AABB.width * 2)) / tileSize);
//	//	}
//	//	else if (m_currentDirection == Direction::Left) 
//	//	{
//	//		fromX = std::floor((m_AABB.left - (m_AABB.width * 2)) / tileSize);
//	//		toX = std::floor((m_AABB.left) / tileSize);
//	//	}
//	//	//Check for tile
//	//	for (int x = fromX; x < toX; ++x)
//	//	{
//	//		if (m_currentDirection == Direction::Right)
//	//		{
//	//			if (x == std::floor(tileBoundary.left / tileSize))
//	//			{
//	//				changeDirection();
//	//				break;
//	//			}
//	//		}
//	//		else if (m_currentDirection == Direction::Left)
//	//		{
//	//			if (x == std::floor((tileBoundary.left + tileBoundary.width) / tileSize))
//	//			{
//	//				changeDirection();
//	//				break;
//	//			}
//	//		}
//	//	}
//	//}
//
//	if (m_collisions.empty()) {
//		return;
//	}
//
//	Map* map = m_sharedContext->m_map;
//	std::sort(m_collisions.begin(), m_collisions.end(), [](CollisionElement* col1, CollisionElement* col2) {return col1->m_area > col2->m_area; });
//
//	//Check for collision on the x axis
//	for (auto &i : m_collisions)
//	{
//		sf::FloatRect intersection = i->m_intersection;
//		sf::FloatRect tileBounds = i->m_collisionBox;
//
//		//Calculate which side of the collision entity is on
//		float xDifference = (m_AABB.left + (m_AABB.width / 2.0f)) - ((tileBounds.left + (tileBounds.width / 2.0f)));
//		float yDifference = (m_AABB.top + (m_AABB.height / 2.0f)) - ((tileBounds.top + (tileBounds.height / 2.0f)));
//		float resolve = 0;
//		if (std::abs(xDifference) > std::abs(yDifference))
//		{
//			changeDirection();
//			break;
//		}
//		else
//		{
//			if (yDifference > 0.0f)
//			{
//				resolve = intersection.height;
//			}
//			else
//			{
//				sf::Vector2i tileUnderEntityPos(std::floor(tileBounds.left), std::floor(tileBounds.top));
//				tileUnderEntityPos.y++;
//
//				if (!map->getTile(tileUnderEntityPos.x, tileUnderEntityPos.y))
//				{
//					resolve = -intersection.height;
//				}
//			}
//			Entity::move(0, resolve);
//			m_velocity.y = 0;
//			break;
//		}
//	}
//}

void Computer::resolveCollisions()
{
	Character::resolveCollisions();

	if (m_collidingOnX)
	{
		changeDirection();
	}
}
