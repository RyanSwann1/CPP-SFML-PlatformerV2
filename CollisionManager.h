#pragma once

#include "EntityType.h"
#include <SFML\Graphics.hpp>
#include <vector>

struct Tile;
struct CollisionElement
{
	CollisionElement(const float area, const sf::FloatRect intersection, const sf::FloatRect collisionBox)
		: m_area(area),
		m_intersection(intersection),
		m_collisionBox(collisionBox)
	{}

	float m_area;
	sf::FloatRect m_intersection;
	sf::FloatRect m_collisionBox;
};

struct SharedContext;
class CollisionManager
{
public:
	CollisionManager(SharedContext* sharedContext);
	~CollisionManager();

	void setEntityType(const EntityType entityType) { m_entityType = entityType; }
	sf::Vector2f update(const sf::FloatRect entity);
	Tile* getReferenceTile() const { return m_referenceTile; }

private:
	SharedContext* m_sharedContext;
	EntityType m_entityType; //Based on the fact that every entity will have this class attached
	Tile* m_referenceTile;

	void checkTileCollisions(const sf::FloatRect entity);

	bool m_collidingOnY;
	bool m_collidingOnX;
};

