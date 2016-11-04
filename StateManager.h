#pragma once

#include "StateBase.h"
#include "StateType.h"
#include "StateMainMenu.h"
#include <unordered_map>
#include <vector>
#include <list>
#include <functional>
#include <SFML\Graphics.hpp>

struct SharedContext;
class StateManager
{
public:
	StateManager(SharedContext* sharedContext);
	~StateManager();

	SharedContext* getSharedContext() { return m_sharedContext; }
	StateBase* getCurrentState() const { return m_currentState; }
	void switchTo(const StateType stateType);
	void removeState(const StateType stateType);
	void update(const float deltaTime);
	void draw(sf::RenderWindow& window);
	sf::View getView() const { return m_view; }

	////Without templates
	//void registerGameState()
	//{
	//	m_stateFactory.emplace(std::make_pair(StateType::Game, [this]() -> StateBase*
	//	{
	//		return new StateGame(this);
	//	}));
	//}
	//void registerMainMenuState()
	//{
	//	m_stateFactory.emplace(std::make_pair(StateType::MainMenu, [this]() -> StateBase*
	//	{
	//		return new StateMainMenu(this);
	//	}));
	//}

	//With templates
	template <class T>
	void registerState(const StateType stateType)
	{
		m_stateFactory.emplace(std::make_pair(stateType, [this]() -> StateBase*
		{
			return new T(this);
		}));
	}

private:
	std::unordered_map<StateType, std::function<StateBase*()>> m_stateFactory;
	//std::vector<std::pair<StateType, StateBase*>> m_states;
	std::list<std::pair<StateType, StateBase*>> m_states;
	std::list<StateType> m_removals;
	StateBase* m_currentState;
	StateType m_currentStateType;
	SharedContext* m_sharedContext;
	sf::View m_view;

	void createState(const StateType stateType);
	void processRemovals();
	bool removeActiveState(const StateType stateType);
};