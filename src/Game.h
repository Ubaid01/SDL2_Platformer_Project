#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <map>
#include <string>

#include "player.h"
#include "platform.h" 
#include "levels.h"

class Game 
{
public:
    Game();
    bool init();
    void run();
    void cleanup();

private:
    bool loadResources();
    void initializeLevelMode();
    void freeResources();
    void loadHighScoreData();
    void saveHighScoreData();
    void addScore(int amount);
    void resetScore();
    void resetGame();
    void GetUserName( SDL_Event& event );
    void instructionsMenu() ;
    void Pause( SDL_Event& event );
    void checkPlayerCollision();
    void DrawFont(const std::string& str, int x, int y, int width, int height, int size, SDL_Color color);
    void renderLevels() ;

    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    Platform platforms[4];
    Levels levels ;

    std::map<std::string , SDL_Texture*> textures;
    std::map<std::string , Mix_Chunk*> sounds;
    std::map < std::string, std::vector<int>> data;
    Mix_Music* backgroundMusic;

    int scoreInt;
    int currentPlatform ;
    std::string name;
    std::string score;
    std::string highscore;
    bool titleScreen;
    bool playCoinFX;
    int mouseDownX;
    int mouseDownY;
    double lavaY;
    double timer;
    double splashTimer;
    double gravity;
    bool firstTime;
    bool playedSplash;
    bool playedSelect;
    bool quit;
    bool mouse_down;
    int mouse_x, mouse_y;
};

#endif // GAME_H
