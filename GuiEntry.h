#pragma once

#include "FontManager.h"
#include "GuiStyle.h"
#include <SFML\Graphics.hpp>
#include <string>

struct GuiEntry
{
public:
	GuiEntry(FontManager* fontManager, const sf::Vector2f& position, const GuiStyle& guiStyle, const std::string& message);
	~GuiEntry();

	std::string getMessage() const { return m_message; }
	std::string getType() const { return m_type; }
	sf::FloatRect getPosition() const;

	void setNewMessage(const std::string& newMessage) { m_message = newMessage; }
	void setPosition(const sf::Vector2f& newPos);

	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape m_shape;
	std::string m_message;
	std::string m_fontName;
	std::string m_type;
	sf::Text m_text;
	FontManager* m_fontManager;
};