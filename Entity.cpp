#include "Entity.h"
#include "Map.h"
#include "SharedContext.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>

Entity::Entity(SharedContext* sharedContext)
	: m_sharedContext(sharedContext),
	m_spriteSheet(sharedContext),
	m_audioPlayer(sharedContext),
	m_collisionManager(sharedContext),
	m_referenceTile(nullptr),
	m_collidingOnX(false),
	m_collidingOnY(false)
{
	m_friction = sf::Vector2f(0.075f, 0);
	m_minFrictionValue = sf::Vector2f(50, 50);
	//m_friction = sharedContext->m_map->getDefaultFriction();
	m_gravity = sf::Vector2f(0, 500);
	//m_gravity = sharedContext->m_map->getDefaultGravity();
}

Entity::~Entity()
{
	TextureManager* textureManager = m_sharedContext->m_textureManager;
	textureManager->releaseResource(m_spriteSheet.getName());
}

sf::Vector2i Entity::getPosition() const
{
	int x = std::floor(m_AABB.left / Sheet::TILE_SIZE);
	int y = std::floor(m_AABB.top / Sheet::TILE_SIZE);
	return sf::Vector2i(x, y);
}

void Entity::addVelocity(const float x, const float y)
{
	m_velocity += sf::Vector2f(x, y);
	
	//Keep m_velocity within confines of the maximum velocity
	if (std::abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x > 0.0f)
		{
			m_velocity.x = m_maxVelocity.x;
		}
		else 
		{
			m_velocity.x = -m_maxVelocity.x;
		}
	}

	if (std::abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y > 0.0f)
		{
			m_velocity.y = m_maxVelocity.y;
		}
		else
		{
			m_velocity.y = -m_maxVelocity.y;
		}
	}
}

void Entity::move(const float x, const float y)
{
	m_oldPosition = m_position;
	m_position += sf::Vector2f(x, y);

	//Check for map boundaries
	sf::Vector2u& maxMapSize = m_sharedContext->m_map->getMaxMapSize();
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}

	//After movement reset velocity
	//m_velocity = sf::Vector2f(0, 0);
}

void Entity::applyFriction(const float deltaTime)
{
	//Apply friction
	if ((std::abs(m_velocity.x) - std::abs(m_friction.x)) > 0.1f)
	{
		if (m_velocity.x > 0.1f)
		{
			m_velocity.x -= m_friction.x;
		}
		else
		{
			m_velocity.x += m_friction.x;
		}
		//m_velocity.x -= x;
	}

	//if (std::abs(m_velocity.x) >= m_minFrictionValue.x)
	//{

	//}

	//https://www.youtube.com/watch?v=lz0YwHFpo-A
}

void Entity::applyGravity()
{
	accelerate(0, m_gravity.y);
}

void Entity::updateAABB()
{
	m_AABB.left = m_position.x + (32 / 2.0f);
	m_AABB.top = m_position.y;
	m_AABB.width = 32 / 2.0f;
	m_AABB.height = Sheet::TILE_SIZE;
}

void Entity::update(const float deltaTime)
{
	applyGravity();
	applyFriction(deltaTime);

	addVelocity(m_acceleration.x * deltaTime, m_acceleration.y * deltaTime);
	setAcceleration(0, 0);

	//Move entity
	sf::Vector2f deltaPos = m_velocity * deltaTime;
	move(deltaPos.x, deltaPos.y);
	
	updateCollisionBox();

	//Update Collision Box
	m_collisionBox.setPosition(sf::Vector2f(m_AABB.left, m_AABB.top));
	m_collisionBox.setSize(sf::Vector2f(m_AABB.width, m_AABB.height));
	m_collisionBox.setFillColor(sf::Color::Blue);
	
	//Collision Checks
	m_collidingOnX = false;
	m_collidingOnY = false;
	checkTileCollisions();
	resolveCollisions();
	//resolveCollisions();
	checkEntityCollisions();
	m_collisions.clear();

	if (!m_collidingOnY)
	{
		m_referenceTile = nullptr;
	}
}

void Entity::draw(sf::RenderWindow & window)
{
	//window.draw(m_collisionShape);
	m_spriteSheet.draw(window);
	for (const auto &i : m_collidedPoints)
	{
		window.draw(i);
	}

	for (const auto &i : m_attackPositions)
	{
		//window.draw(i);
	}
	//window.draw(m_collisionBox);
}

//void Entity::resolveCollisions()
//{
//	Map* map = m_sharedContext->m_map;
//	if (m_collisions.empty()) {
//		return;
//	}
//
//	std::sort(m_collisions.begin(), m_collisions.end(), [](CollisionElement* col1, CollisionElement* col2) {return col1->m_area > col2->m_area; });
//
//	for (auto &i : m_collisions)
//	{
//		//Check to see if entity is still colliding with collision in question
//		if (!m_AABB.intersects(i->m_collisionBox)) {
//			continue;
//		}
//
//		sf::FloatRect collisionBox = i->m_collisionBox;
//		//float xDifference = (m_AABB.left + (m_AABB.width / 2.0f)) - (collisionBox.left + (collisionBox.width / 2.0f));
//		float yDifference = (m_AABB.top + (m_AABB.height / 2.0f)) - (collisionBox.top + (collisionBox.height / 2.0f));
//		float resolve = 0;
//		sf::FloatRect intersection = i->m_intersection;
//		if (yDifference > 0) {
//			resolve = intersection.height;
//		}
//		else 
//		{
//			sf::Vector2i tileUnderEntityPos(std::floor(collisionBox.left), std::floor(collisionBox.top));
//			tileUnderEntityPos.y++;
//			
//			if (!map->getTile(tileUnderEntityPos.x, tileUnderEntityPos.y))
//			{
//				resolve = -intersection.height;
//			}
//		}
//
//		move(0, resolve);
//		m_velocity.y = 0;
//		break;
//
//	}
//}


void Entity::checkTileCollisions()
{
	int tileSize = Sheet::TILE_SIZE;
	int x = std::floor(m_AABB.left / tileSize);
	int y = std::floor((m_AABB.top + m_AABB.height) / tileSize);
	Map* map = m_sharedContext->m_map;

	Tile* tile = map->getTile(x, y);
	if (tile)
	{
		sf::Vector2i tilePos = tile->m_tileInfo->m_position;
		sf::FloatRect tileBoundary(tilePos.x * tileSize, tilePos.y * tileSize, tileSize, tileSize);
		sf::FloatRect intersection;
		if (m_AABB.intersects(tileBoundary, intersection))
		{
			float area = intersection.height * intersection.width;
			m_collisions.emplace_back(new CollisionElement(area, intersection, tileBoundary));

			//Debug
			sf::RectangleShape rect;
			rect.setPosition(tileBoundary.left, tileBoundary.top);
			rect.setSize(sf::Vector2f(tileSize, tileSize));
			rect.setFillColor(sf::Color::Green);
			m_attackPositions.push_back(rect);
		}
	}
}

void Entity::updateCollisionBox()
{
	//Hacky!!
	updateAABB();
	switch (m_currentDirection)
	{
	case (Direction::Right):
	{
		m_AABB.left = m_AABB.left + (Sheet::TILE_SIZE / 6.0f);
		break;
	}
	case (Direction::Left):
	{
		m_AABB.left = m_AABB.left - (Sheet::TILE_SIZE / 2.0f);
		break;
	}
	}
}

void Entity::checkEntityCollisions()
{
	//Search for collision
	sf::Vector2i entityPos = getPosition();
	EntityManager* entityManager = m_sharedContext->m_entityManager;
	Entity* apposingEntity = entityManager->getEntityAtPosition(entityPos);
	//Apply the collision if apposing entity found
	if (apposingEntity)
	{
		this->onEntityCollision(apposingEntity);
		apposingEntity->onEntityCollision(this);
	}
}

void Entity::resolveCollisions()
{
	if (m_collisions.empty()) {
		return;
	}

	std::sort(m_collisions.begin(), m_collisions.end(), [](CollisionElement* col1, CollisionElement* col2) {return col1->m_area > col2->m_area; });
	Map* map = m_sharedContext->m_map;

	for (const auto &i : m_collisions)
	{
		sf::FloatRect intersection = i->m_intersection;
		sf::FloatRect tileBounds = i->m_collisionBox;

		float xDifference = (m_AABB.left + (m_AABB.width / 2.0f)) - (tileBounds.left + (tileBounds.width / 2.0f));
		float yDifference = (m_AABB.top + (m_AABB.height / 2.0f)) - (tileBounds.top + (tileBounds.height / 2.0f));
		float resolve = 0;
		//Colliding on the X axis
		if (std::abs(xDifference) > std::abs(yDifference))
		{
			if (xDifference > 0)
			{
				resolve = intersection.width;
			}
			else
			{
				resolve = -intersection.width;
			}
			m_collidingOnX = true;
			move(resolve, 0);
			m_velocity.x = 0;
		}
		//Colliding on the y axis
		else
		{
			if (yDifference > 0)
			{
				resolve = intersection.height;
			}
			else
			{
				resolve = -intersection.height;
			}
			m_collidingOnY = true;
			move(0, resolve);
			m_velocity.y = 0;
		}

		if (m_collidingOnY)
		{
			Tile* tile = map->getTile(std::floor(tileBounds.left / Sheet::TILE_SIZE), std::floor(tileBounds.top / Sheet::TILE_SIZE));
			if (tile)
			{
				m_referenceTile = tile;
			}
		}
	}
}
