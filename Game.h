#pragma once

#include "Window.h"
#include "TextureManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EntityManager.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GUIManager.h"
#include "FontManager.h"

class Game
{
public:
	Game();
	~Game();

	bool isRunning() const { return m_window.isRunning(); }

	void update();
	void draw();
	void lateUpdate();

private:
	Window m_window;
	TextureManager m_textureManager;
	SharedContext m_sharedContext;
	StateManager m_stateManager;
	EntityManager m_entityManager;
	AudioManager m_audioManager;
	GUIManager m_guiManager;
	FontManager m_fontManager;
	sf::Time m_timeElasped;
	sf::Clock m_clock;
};

