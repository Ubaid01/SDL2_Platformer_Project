#include "levels.h"
#include "platform.h"

Levels::Levels(int x)
	:totalLevels(x), currentLevel( -1 ), width( 200 ) , height( 80 )
{
	int dy = ( SCREEN_HEIGHT - height * totalLevels ) / ( totalLevels + 1 ) ;
	int y = dy;
	for (int i = 0; i < totalLevels; i++) {
		SDL_Rect destRect = { SCREEN_WIDTH / 2 - width / 2 , y , width , height } ;
		y += height + dy ;
		vectorLevels.push_back( destRect ) ;
	}
}

void Levels::inputLevelMode( SDL_Event& event , bool& exit )
{
	int mouse_x, mouse_y;
	while ( !isLevelSelected() )
	{
		while (SDL_PollEvent(&event))
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
			if ( event.type == SDL_MOUSEBUTTONDOWN )
			{
				for (int i = 0; i < totalLevels; i++)
				{
					if ( mouse_x > vectorLevels[i].x && mouse_x < vectorLevels[i].x + vectorLevels[i].w && mouse_y > vectorLevels[i].y && mouse_y < vectorLevels[i].y + vectorLevels[i].h )
						currentLevel = i + 1 ;
				}
			}
			if ( event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
			{
				exit = true ;
				currentLevel = -2;
			}
		}
	}
}


