#include "CollisionManager.h"
#include "SharedContext.h"


CollisionManager::CollisionManager(SharedContext* sharedContext)
	: m_sharedContext(sharedContext)
{
}


CollisionManager::~CollisionManager()
{
}

sf::Vector2f CollisionManager::update(const sf::FloatRect entity)
{
	return sf::Vector2f();
}

void CollisionManager::checkTileCollisions(const sf::FloatRect entity)
{
}
