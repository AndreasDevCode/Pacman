#include "Game.h"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();
	
	// if initializing is successfull game continue;
	if (success)
	{
		game.RunLoop();
	}

	// out of the loop close the game
	game.Shutdown();

	return 0;
}


