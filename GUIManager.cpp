#include "GUIManager.h"
#include <iostream>


GUIManager::GUIManager(SharedContext* sharedContext)
	: m_sharedContext(sharedContext),
	m_currentInterface(nullptr)
{
}


GUIManager::~GUIManager()
{
}

void GUIManager::addInterface(const std::string & interfaceName, const GUIInterface& guiInterface)
{
	auto iter = findInterface(interfaceName);
	if (!iter)
	{
		m_guiInterfaces.emplace(std::make_pair(interfaceName, guiInterface));
	}
	else {
		std::cerr << "Interface already exists.\n";
	}
}

void GUIManager::removeInterface(const std::string & interfaceName)
{
	auto iter = m_guiInterfaces.find(interfaceName);
	if (iter != m_guiInterfaces.cend())
	{
		m_guiInterfaces.erase(iter);
	}
}

void GUIManager::setCurrentInterface(const std::string & interfaceName)
{
	auto iter = findInterface(interfaceName);
	if (iter)
	{
		m_currentInterface = iter;
		int i = 0;
	}
}

void GUIManager::setInterfaceVisibility(const std::string & interfaceName, const bool visible)
{
	auto iter = findInterface(interfaceName);
	if (iter)
	{
		iter->setVisibility(visible);
	}
}

void GUIManager::draw(sf::RenderWindow & window)
{
	if (m_currentInterface)
	{
		m_currentInterface->draw(window);
	}
	//Check for 
}

bool GUIManager::interactingWithGuiEntry(const sf::Vector2i & mousePos, const std::string& message)
{
	GuiEntry* guiEntry = m_currentInterface->getGui(message);

	sf::FloatRect guiEntryRect = guiEntry->getPosition();
	
	if (mousePos.x >= guiEntryRect.left &&
		mousePos.y >= guiEntryRect.top &&
		mousePos.x <= (guiEntryRect.left + guiEntryRect.width) &&
		mousePos.y <= (guiEntryRect.top + guiEntryRect.height))
	{
		return true;
	}
	//Debug
	//else
	//{
	//	std::cout << "Mouse position x: " << mousePos.x << "\n";
	//	std::cout << guiEntryRect.left << "\n";
	//	std::cout << "Mouse Position y: " << mousePos.y << "\n";
	//	std::cout << guiEntryRect.top << "\n";
	//}
	
	return false;
}

GUIInterface * GUIManager::findInterface(const std::string & interfaceName)
{
	auto cIter = m_guiInterfaces.find(interfaceName);
	return (cIter != m_guiInterfaces.cend() ? &cIter->second : nullptr);
}
