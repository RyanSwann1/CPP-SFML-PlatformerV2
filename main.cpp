#include "Game.h"

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>

int main()
{
	Game game;
	while (game.isRunning())
	{
		game.update();
		game.draw();
		game.lateUpdate();
	}

	return 0;
}

