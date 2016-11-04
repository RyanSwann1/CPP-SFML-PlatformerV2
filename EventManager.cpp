#include "EventManager.h"
#include <sstream>
#include <fstream>
#include <iostream>

EventManager::EventManager(const std::string& filePath)
{
	loadIn(filePath);
	//m_currentState = StateType::Game; //Need to change

}

EventManager::~EventManager()
{
}

void EventManager::update(const sf::Event currentEvent)
{
	//Search through all available bindings
	for (auto &i : m_bindings)
	{
		if (i.m_eventDetails.m_eventType == static_cast<EventType>(currentEvent.type))
		{
			if (i.m_eventDetails.m_eventInfo.m_keyCode == currentEvent.key.code)
			{
				activateCallBack(i);
			}
		}
	}
}

void EventManager::removeCallback(const std::string & name)
{
	auto iter = m_callBacks.find(name);
	if (iter != m_callBacks.cend())
	{
		m_callBacks.erase(iter);
	}
}

void EventManager::loadIn(const std::string & filePath)
{
	std::ifstream file;
	file.open(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << "\n";
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string name;
		int type = 0;
		int keyCode = 0;

		keyStream >> name >> type >> keyCode;

		Binding bind(name, type, keyCode);
		addBinding(name, bind);
	}
	file.close();
}

void EventManager::addBinding(const std::string& name, const Binding & binding)
{
	bool addBinding = true;
	for (auto &i : m_bindings)
	{
		if (i.m_name == name)
		{
			addBinding = false;
		}
	}

	//Binding doesn't exist
	if (addBinding) {
		m_bindings.emplace_back(binding);
	}
}

void EventManager::activateCallBack(const Binding& binding)
{
	//Activate callback if found
	auto iter = m_callBacks.find(binding.m_name);
	if (iter != m_callBacks.cend())
	{
		//Check if current state matches one requested of callback
		if (iter->second.first == m_currentState)
		{
			iter->second.second(&binding.m_eventDetails);
		}
		//Send in appropriate binding 
		//call the callback function
	}
}
