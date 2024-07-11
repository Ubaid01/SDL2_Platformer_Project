#ifndef LEVELS_H
#define LEVELS_H

#include <SDL.h>
#include <vector>

class Levels
{	
public:
	Levels(int x);
	SDL_Rect& getLevelRect(int index) { return vectorLevels[index] ; }
	bool isLevelSelected() { return ( currentLevel != -1 ) ; }
	void inputLevelMode( SDL_Event& event , bool& exit );
	int getTotalLevels() { return totalLevels; }
	int getCurrentLevel() { return currentLevel; }
	
private:
	std::vector<SDL_Rect> vectorLevels;
	int width;
	int height;
	int totalLevels;
	int currentLevel;
};

#endif // LEVELS_H
