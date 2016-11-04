#pragma once

#include "GUIInterface.h"
#include <unordered_map>

struct SharedContext;
class GUIManager
{
public:
	GUIManager(SharedContext* sharedContext);
	~GUIManager();

	void addInterface(const std::string& interfaceName, const GUIInterface& guiInterface);
	void removeInterface(const std::string& interfaceName);
	void setCurrentInterface(const std::string& interfaceName);
	void setInterfaceVisibility(const std::string& interfaceName, const bool visible);

	bool interactingWithGuiEntry(const sf::Vector2i& mousePos, const std::string& message);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<std::string, GUIInterface> m_guiInterfaces;
	SharedContext* m_sharedContext;
	GUIInterface* m_currentInterface;

	GUIInterface* findInterface(const std::string& interfaceName);
};

