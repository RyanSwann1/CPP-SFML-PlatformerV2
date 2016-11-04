#pragma once

#include <SFML\Graphics.hpp>

struct EventDetails;
class StateManager;
class StateBase
{
public:
	StateBase(StateManager* stateManager)
	: m_stateManager(stateManager) {}

	virtual ~StateBase() {}

	virtual void onEnter() {}
	virtual void onExit() {}

	virtual void onCreate() {}
	virtual void onDestroy() {}

	virtual void update(const float deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

	sf::View getView() const { return m_view; }

protected:
	StateManager* m_stateManager;
	sf::View m_view;
	virtual void reactToInput(const EventDetails* eventDetails) {}

private:
	StateBase(const StateBase&);
	StateBase& operator=(const StateBase&);
};

