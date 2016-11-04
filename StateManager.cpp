#include "StateManager.h"
#include "SharedContext.h"
#include "Window.h"
#include "StateGame.h"
#include "StateMainMenu.h"
#include "StateEndGame.h"
#include <algorithm>

StateManager::StateManager(SharedContext* sharedContext)
	: m_sharedContext(sharedContext),
	m_currentState(nullptr)
{
	registerState<StateGame>(StateType::Game);
	registerState<StateMainMenu>(StateType::MainMenu);
	registerState<StateEndGame>(StateType::EndGame);
}

StateManager::~StateManager()
{
	for (auto &i : m_stateFactory)
	{
		delete i.second();
		i.second = nullptr;
	}
	m_stateFactory.clear();
}

void StateManager::switchTo(const StateType stateType)
{
	//Iterate through existing states to see if state already exists
	if (!m_states.empty())
	{
		for (auto iter = m_states.begin(); iter != m_states.end(); ++iter)
		{
			if (stateType == iter->first)
			{
				m_currentState->onExit();
				StateType tempType = iter->first;
				StateBase* tempState = iter->second;
				m_states.emplace_back(std::make_pair(tempType, tempState));
				m_currentState = m_states.back().second;
				m_currentState->onEnter();
				return;
			}
		}
	}

	//Exit current state
	if (m_currentState)
	{
		m_currentState->onExit();
	}

	createState(stateType);
	m_currentState->onEnter();
	m_sharedContext->m_window->getWindow().setView(m_view);
	m_currentStateType = stateType;
	m_sharedContext->m_window->getEventManager().setCurrentState(m_currentStateType);
}

void StateManager::removeState(const StateType stateType)
{
	if (std::find(m_removals.cbegin(), m_removals.cend(), stateType) == m_removals.cend())
	{
		m_removals.push_back(stateType);
	}
}

void StateManager::update(const float deltaTime)
{
	m_currentState->update(deltaTime);
}

void StateManager::draw(sf::RenderWindow & window)
{
	m_currentState->draw(window);
}

void StateManager::createState(const StateType stateType)
{
	//Create new state
	auto iter = m_stateFactory.find(stateType);
	if(iter != m_stateFactory.cend())
	m_states.emplace_back(std::make_pair(iter->first, iter->second()));
	m_currentState = m_states.back().second; 
	m_currentState->onCreate();
}

void StateManager::processRemovals()
{
	for (auto iter = m_removals.begin(); iter != m_removals.end();)
	{
		if (removeActiveState(*iter))
		{
			iter = m_removals.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
}

bool StateManager::removeActiveState(const StateType stateType)
{
	for (auto iter = m_states.begin(); iter != m_states.end();)
	{
		if (iter->first == stateType)
		{
			iter->second->onDestroy();
			m_states.erase(iter);
			return true;
		}
		else
		{
			++iter;
		}
	}
	return false;
}