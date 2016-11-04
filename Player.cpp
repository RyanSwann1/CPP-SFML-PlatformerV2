#include "Player.h"
#include "EventManager.h"
#include "SharedContext.h"
#include "EntityManager.h"
#include "AudioManager.h"
#include <iostream>

Player::Player(SharedContext* sharedContext)
	: Character(sharedContext)
{
	m_type = EntityType::Player;
	m_name = "Player";
	Character::load(m_sharedContext->m_entityManager->getEntityTypeLocation(m_name));
	setDirection(Direction::Right);
	

	m_audioPlayer.registerAudioClip(m_name, "Attack");
	m_audioPlayer.registerAudioClip(m_name, "Jump");
	m_audioPlayer.registerAudioClip(m_name, "Death");
	m_audioPlayer.registerOwner(m_name);

	EventManager* eventManager = m_sharedContext->m_eventManager;
	eventManager->addCallback<Player>("Move_Left", StateType::Game, &Player::reactToInput, this);
	eventManager->addCallback<Player>("Move_Right", StateType::Game, &Player::reactToInput, this);
	eventManager->addCallback<Player>("Jump", StateType::Game, &Player::reactToInput, this);
	eventManager->addCallback<Player>("Action_Button", StateType::Game, &Player::reactToInput, this);
}

Player::~Player()
{
	EventManager* eventManager = m_sharedContext->m_eventManager;
	eventManager->removeCallback("Move_Left");
	eventManager->removeCallback("Move_Right");
	eventManager->removeCallback("Jump");
	eventManager->removeCallback("Action_Button");
}

void Player::onEntityCollision(Entity * entity)
{
	if (entity->getType() == EntityType::Enemy)
	{
		Character* character = static_cast<Character*>(entity);
		applyDamage(character->getDamageValue());
	}
}

void Player::reactToInput(const EventDetails * eventDetails)
{
	if (eventDetails->m_name == "Move_Left")
	{
		move(Direction::Left);
	}
	else if (eventDetails->m_name == "Move_Right")
	{
		move(Direction::Right);
	}
	else if (eventDetails->m_name == "Action_Button")
	{
		attackPreperation();
	}
	else if (eventDetails->m_name == "Jump")
	{
		jump();
	}
}
