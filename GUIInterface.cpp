#include "GUIInterface.h"
#include "FontManager.h"
#include <algorithm>
#include <iostream>


GUIInterface::GUIInterface(const std::string& name)
	: m_name(name),
	m_isVisible(false)
{
	//Copy over entries into here
}

GUIInterface::~GUIInterface()
{}

GuiEntry* GUIInterface::getGui(const std::string & message)
{
	auto iter = std::find_if(m_entries.begin(), m_entries.end(), [message](GuiEntry* guiEntry) {return guiEntry->getMessage() == message; });
	if (iter != m_entries.end())
	{
		return *iter;
	}

	else {
		std::cout << "Couldn't find message.\n";
	}
}

void GUIInterface::changeMessage(const std::string & existingMessage, const std::string & newMessage)
{
	auto iter = std::find_if(m_entries.begin(), m_entries.end(), [existingMessage](GuiEntry* guiEntry) {return guiEntry->getMessage() == existingMessage; });
	if (iter != m_entries.end())
	{
		(*iter)->setNewMessage(newMessage);
	}
	else {
		std::cout << "Couldn't find existing message.\n";
	}
}

void GUIInterface::draw(sf::RenderWindow & window)
{
	if (m_isVisible)
	{
		for (auto &i : m_entries)
		{
			i->draw(window);
		}
	}
}

void GUIInterface::constructNewGuiElement(FontManager* fontManager, const GuiStyle & guiStyle, const std::string & message, const sf::Vector2f & position)
{
	bool addGuiElement = true;
	for (auto cIter = m_entries.cbegin(); cIter != m_entries.cend(); ++cIter)
	{
		if ((*cIter)->getMessage() == message)
		{
			addGuiElement = false;
		}
	}
	
	m_entries.emplace_back(new GuiEntry(fontManager, position, guiStyle, message));
}