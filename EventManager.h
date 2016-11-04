#pragma once

#include "StateType.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

enum class EventType
{
	KeyPressed = sf::Event::KeyPressed,
	KeyReleased = sf::Event::KeyReleased,
	MouseButtonPressed = sf::Event::MouseButtonPressed
};

struct EventInfo
{
	EventInfo(int i)
		: m_keyCode(i) {}

	int m_keyCode;
};

struct EventDetails
{
	EventDetails(const std::string& name, const int eventType, const int keyCode)
		: m_name(name),
		m_eventInfo(keyCode),
		m_eventType(static_cast<EventType>(eventType)) {}

	std::string m_name;
	EventInfo m_eventInfo;
	EventType m_eventType;
};

struct Binding
{
	Binding(const std::string& name, const int eventType, const int keyCode)
		: m_name(name),
		m_eventDetails(name, eventType, keyCode)
		{}

	std::string m_name;
	EventDetails m_eventDetails;
};

using namespace std::placeholders;
using CallBackContainer = std::pair<StateType, std::function<void(const EventDetails*)>>;

class EventManager
{
public:
	EventManager(const std::string& filePath);
	~EventManager();

	void setCurrentState(const StateType stateType) { m_currentState = stateType; }
	void update(const sf::Event currentEvent);
	void removeCallback(const std::string& name);

	template <class T>
	void addCallback(const std::string& name, const StateType stateType, void(T::*fp)(const EventDetails*), T* instance)
	{
		if (m_callBacks.find(name) == m_callBacks.cend())
		{
			auto callBackContainer = std::make_pair(stateType, std::bind(fp, instance, _1));
			m_callBacks.emplace(name, callBackContainer);
		}
	}
	
	//Not sure if this is in the right place
	//sf::Vector2i getMousePos(const sf::RenderWindow& window) const { return sf::Mouse::getPosition(window); }

private:
	std::unordered_map<std::string, CallBackContainer> m_callBacks;
	std::vector<Binding> m_bindings;

	StateType m_currentState;
	void loadIn(const std::string& filePath);
	void addBinding(const std::string& name, const Binding& binding);
	void activateCallBack(const Binding& binding);

};