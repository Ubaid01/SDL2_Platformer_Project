#include "Game.h"

int main(int argc, char* argv[])
{
    Game game;

    if ( !game.init() ) 
	{
		SDL_Log("\nUnable to initialize the game!");
        return -1;
    }

    game.run();
    game.cleanup();


    return 0;
}
