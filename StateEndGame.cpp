#include "StateEndGame.h"
#include "SharedContext.h"
#include "StateManager.h"
#include "Window.h"

StateEndGame::StateEndGame(StateManager* stateManager)
	: StateBase(stateManager)
{
}


StateEndGame::~StateEndGame()
{
}

void StateEndGame::onCreate()
{
	Window* window = m_stateManager->getSharedContext()->m_window;
	window->close();
	window->stopRunning();
}
