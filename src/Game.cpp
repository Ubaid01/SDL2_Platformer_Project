#include "Game.h"
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

Game::Game()
    : window(nullptr), renderer(nullptr), backgroundMusic( nullptr ) , 
    player(0, 0, 26, 32), levels(2) , scoreInt(0), currentPlatform( -1 ) 
    , name("") , score("000"), highscore("BEST: 0"), titleScreen(true), 
    playCoinFX(false), mouseDownX(0), mouseDownY(0), lavaY(SCREEN_HEIGHT - 32), 
    timer(0) ,splashTimer(0), firstTime(true), gravity( STARTING_GRAVITY ) , 
    playedSplash(false), playedSelect(false), quit(false), mouse_down(false),
    mouse_x(0), mouse_y(0)
{
    srand(static_cast<unsigned int>( time (nullptr ) ) ) ;
}

void Game::initializeLevelMode()
{
    gravity = STARTING_GRAVITY;

    for (int i = 0; i < 4; i++)
        platforms[i] = Platform(i, levels.getCurrentLevel());

    player.setVelocity(0, 0);
    player.setX( platforms[0].getX() + platforms[0].getWidth() / 2 - 13 ) ;
    player.setY( platforms[0].getY() - platforms[0].getHeight() );
}

bool Game::init() 
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "Game subsystem unable to initialize! " << SDL_GetError() << std::endl;
        return false;
    }
    if ( !IMG_Init(IMG_INIT_PNG) )
    {
        std::cerr << "PNG OR JPG Initialization has Failed! " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to load font! " << TTF_GetError() << std::endl;
        return false;
    }

    if ( Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) == -1 )
    {
        std::cerr << "Couldn't initialize the audio subsystem! " << Mix_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Platform Prodigy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    if (window == nullptr || renderer == nullptr)
    {
        std::cerr << "Window and Render unable to setup properly. " << SDL_GetError() << std::endl;
        return false;
    }

    if (!loadResources())
    {
		std::cerr << "Unable to load resources! " << SDL_GetError() << std::endl;
		return false ;
    }
	loadHighScoreData() ;
    resetScore();

    SDL_Surface* surface = IMG_Load("resources/icon.ico");
    if ( !surface )
    {
		std::cerr << "Unable to load the window icon! " << IMG_GetError() << std::endl;
		return false;
    }
    SDL_SetWindowIcon(window , surface);
    SDL_FreeSurface(surface);

    return true;
}

bool Game::loadResources() 
{
	textures["player"] = IMG_LoadTexture(renderer, "resources/egg.png") ;
	textures["lava"] = IMG_LoadTexture(renderer, "resources/lava.png") ;
	textures["platform"] = IMG_LoadTexture(renderer, "resources/platform.png") ;
	textures["coin"] = IMG_LoadTexture(renderer, "resources/coin.png") ;
	textures["scoreBox"] = IMG_LoadTexture(renderer, "resources/scorebox.png") ;
	textures["logo"] = IMG_LoadTexture(renderer, "resources/logo.png") ;
	textures["splashEgg"] = IMG_LoadTexture(renderer, "resources/splash_egg.png") ;
	textures["mouse"] = IMG_LoadTexture( renderer , "resources/comp_mouse.png" ) ;
	textures["space"] = IMG_LoadTexture( renderer , "resources/space_key.png" ) ;
	textures["escape"] = IMG_LoadTexture( renderer , "resources/escape_key.png" ) ;
    textures["p_key"] = IMG_LoadTexture(renderer, "resources/p_key.png");

	sounds["launch"] = Mix_LoadWAV("resources/launch.wav") ;
	sounds["aim"] = Mix_LoadWAV("resources/click.wav") ;
	sounds["death"] = Mix_LoadWAV("resources/die.wav") ;
	sounds["coin"] = Mix_LoadWAV("resources/coin.wav") ;
	sounds["splash"] = Mix_LoadWAV("resources/splash.wav") ;
	sounds["select"] = Mix_LoadWAV("resources/select.wav") ;
	backgroundMusic = Mix_LoadMUS("resources/A tricky puzzle_1.ogg") ;

    if ( !backgroundMusic )
    {
		std::cerr << "Unable to load background music! " << Mix_GetError() << std::endl;
        return false ;
    }
    Mix_PlayMusic( backgroundMusic, -1 ) ;

    for (const auto& texture : textures)
    {
        if (texture.second == nullptr)
        {
			std::cerr << "Unable to initialize texture " << texture.first << std::endl;
            return false;
        }

    }

    for (const auto& sound : sounds )
    {
        if (sound.second == nullptr)
        {
            std::cerr << "Unable to initialize texture " << sound.first << std::endl;
            return false;
        }
    }

    return true;
}

void Game::freeResources() 
{
    for (auto& texturePair : textures) 
        SDL_DestroyTexture(texturePair.second);
    textures.clear();
    
    for (auto& soundPair : sounds) 
        Mix_FreeChunk(soundPair.second);
    sounds.clear();
    
    Mix_FreeMusic( backgroundMusic ) ;
    data.clear() ;
    if (renderer) 
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) 
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void Game::cleanup() 

{
    freeResources();

    Mix_CloseAudio();
    TTF_Quit();
    Mix_Quit() ;
    IMG_Quit();
    SDL_Quit();
}

void Game::loadHighScoreData() 
{
    std::ifstream scorefile("highscore.bin", std::ios::binary);

    if ( scorefile.is_open() )
    {
        while ( !scorefile.eof() )
        {
            std::string name;
            size_t name_length;
            int num_modes = levels.getTotalLevels() ;
            std::vector<int> scores( num_modes, 0 ) ;

            scorefile.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
            name.resize(name_length);
            scorefile.read(&name[0], name_length);

            for ( int i = 0; i < num_modes; i++ )
                scorefile.read(reinterpret_cast<char*>(&scores[i]), sizeof(scores[i]) ) ;

            data[name] = scores ;
        }
        scorefile.close();
    }
    else
        std::cerr << "Unable to load data from highscore.bin! " << SDL_GetError() << std::endl;
}

void Game::saveHighScoreData() 
{
    std::ofstream scorefile("highscore.bin", std::ios::binary) ;

    if ( scorefile.is_open() )
    {
        for ( const auto& pair : data )
        {
            const std::string& name = pair.first ;
            const std::vector<int>& scores = pair.second ;

            size_t name_length = name.size() ;
            scorefile.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
            scorefile.write(name.c_str(), name_length);

            for (const int score : scores)
                scorefile.write(reinterpret_cast<const char*>(&score), sizeof(score) ) ;
        }
        scorefile.close();
    }
    else
        std::cerr << "Unable to save data to highscore.bin! " << SDL_GetError() << std::endl;
}

void Game::addScore(int amount) 
{
    scoreInt += amount;

    if (scoreInt < 10)
        score = "00" + std::to_string(scoreInt);
    else if (scoreInt < 100)
        score = "0" + std::to_string(scoreInt);
    else
        score = std::to_string(scoreInt);

   	if ( scoreInt > data[name][ levels.getCurrentLevel( ) - 1 ])
	{
        data[name][ levels.getCurrentLevel( ) - 1 ] = scoreInt;
        highscore = "BEST: " + std::to_string(data[name][ levels.getCurrentLevel( ) - 1 ]);
    }
    if ( scoreInt % 5 == 0 && levels.getCurrentLevel() == LM_EasyMode )
        gravity += 0.001 ;
    else if ( levels.getCurrentLevel() == LM_AdventureMode )
        gravity += 0.01 ;
}

void Game::resetScore() 
{
    scoreInt = 0;
    score = "000";
    saveHighScoreData( ) ;
}

void Game::resetGame()
{
    resetScore();
    initializeLevelMode();
}

void Game::Pause( SDL_Event& event ) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, nullptr);

    DrawFont( "Paused", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 50 , 160 ,50 ,50 , { 128, 213, 90 } ) ;
    DrawFont( "Press Escape key OR close button of window to exit anytime.", SCREEN_WIDTH / 3 - 110, SCREEN_HEIGHT - 100 , 500 , 40 , 250, { 128, 213, 90 } ) ;

    SDL_RenderPresent(renderer);
    bool isPause = true ;
    while ( isPause ) 
	{
		while( SDL_PollEvent(&event) )
		{
			if (event.type == SDL_QUIT || ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_SPACE))) 
			{
				SDL_RenderClear( renderer ) ;
                isPause = false ;
                SDL_Delay(16);  // Delay to avoid busy-waiting
        	}
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
            {
                if ( Mix_PausedMusic() != 1 )
                    Mix_PauseMusic();
                else
                    Mix_ResumeMusic();
            }
		}
    }
}

void Game::checkPlayerCollision() 
{
    bool onPlatform = false;
    for (int i = 0; i < 4; i++) 
	{
        if (platforms[i].getHasCoin() && player.getX() + player.getWidth() - 3 > platforms[i].getCoinX() && player.getX() + 3 < platforms[i].getCoinX() + 24 && player.getY() + player.getHeight() - 3 > platforms[i].getCoinY() && player.getY() + 3 < platforms[i].getCoinY() + 24) {
            addScore(1);
            platforms[i].setHasCoin(false);
            playCoinFX = true;
        }

        if (player.getX() + 2 < platforms[i].getX() + platforms[i].getWidth() && player.getX() + player.getWidth() > platforms[i].getX() && player.getY() + player.getHeight() >= platforms[i].getY() && player.getY() < platforms[i].getY() + platforms[i].getHeight()) {
            if (player.getY() > platforms[i].getY() + platforms[i].getHeight() / 2) 
			{
                player.setVelocity(player.getVelocity().x, 5);
            }
            else if (player.getY() + player.getHeight() < platforms[i].getY() + platforms[i].getHeight()) 
			{
                onPlatform = true;
            	currentPlatform = i ;
                player.setY(platforms[i].getY() - player.getHeight() + 1);
            }
        }
        
        for ( int j = i + 1; j < 4; j++ ) 
        {
            if (platforms[i].checkOverlap(platforms[i].getX(), platforms[i].getY(), platforms[i].getWidth(), platforms[i].getHeight(),
                                          platforms[j].getX(), platforms[j].getY(), platforms[j].getWidth(), platforms[j].getHeight())) 
            {
                platforms[j].block(); 
            }
        }
    }
    player.setOnPlatform(onPlatform);
    
    // Setup transformed platform motion
    if( player.isOnPlatform() ) 
    {
    	if( platforms[ currentPlatform ].isPlatformHaveMotion(MT_Swaying) || platforms[currentPlatform].isPlatformHaveMotion(MT_Diagonal) )
    	{
    		player.setX( player.getX() + platforms[ currentPlatform ].getChangedX() ) ; // Changed player's X coordinate based on the platform's dx.
		}
		else if( platforms[ currentPlatform].isPlatformHaveMotion(MT_Parabolic) )
		{
			player.setX( player.getX() + platforms[ currentPlatform ].getChangedX() ) ; 
            if ( player.getVelocity().y <= 6 && player.getVelocity().y >= -6 )
                player.setY( player.getY() + platforms[ currentPlatform ].getChangedY() ) ;
		}
	}

}

void Game::DrawFont(const std::string& str, int x, int y, int width, int height, int size, SDL_Color color) 
{
    TTF_Font* font = TTF_OpenFont("resources/font.otf", size);

    SDL_Surface* message_surf = TTF_RenderText_Blended(font, str.c_str(), color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, message_surf);
    SDL_Rect Message_rect = { x, y, width, height };
    SDL_RenderCopy(renderer, Message, nullptr, &Message_rect);

    SDL_DestroyTexture(Message);
    SDL_FreeSurface(message_surf);
    TTF_CloseFont(font);
}

void Game::renderLevels() 
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 255);
    SDL_RenderClear(renderer);

    std::vector<std::string> modes = { "Easy Mode" , "Adventure Mode" } ;

    for (int i = 0; i < levels.getTotalLevels(); i++) 
	{
        SDL_RenderCopy(renderer, textures["scoreBox"], nullptr, &levels.getLevelRect(i) );
        DrawFont( modes[i] , levels.getLevelRect(i).x + 25, levels.getLevelRect(i).y + 15, levels.getLevelRect(i).w - 50, levels.getLevelRect(i).h - 30, levels.getLevelRect(i).h - 30, {0, 0, 0} ) ;
    }
    DrawFont( "Click to Select Mode", SCREEN_WIDTH / 3 + 5 , SCREEN_HEIGHT / 6 - 40 , 250, 40, 200, { 128 , 128 , 128 } ) ;

    SDL_RenderPresent(renderer);
}

void Game::GetUserName( SDL_Event& event ) 
{
    SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
    SDL_StartTextInput();
	
	bool nameInput = false ;
    while ( !nameInput ) 
	{
        SDL_RenderClear(renderer);
        SDL_Rect logo_rect = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 45 - 30, 400, 90 };
        SDL_RenderCopy(renderer, textures["logo"], nullptr, &logo_rect);
        DrawFont("Enter your name", SCREEN_WIDTH / 2 - 135, SCREEN_HEIGHT / 2 + 50, 270, 32, 32, { 0, 0, 0 } ) ;
        DrawFont(name, SCREEN_WIDTH / 2 - ( name.length() * 15 ) / 2 , SCREEN_HEIGHT / 2 + 50 + 40 , name.length() * 15 , 32, 32, {178, 150, 125} ) ;
        SDL_RenderPresent(renderer);

        while ( SDL_PollEvent(&event) ) 
		{
            if ( event.type == SDL_QUIT )
			{
				name = "" ;
				SDL_StopTextInput();
				nameInput = true ;
				quit = true ;
			} 
            
            if ( event.type == SDL_KEYDOWN ) {
                if ( event.key.keysym.sym == SDLK_RETURN )
                {
                	SDL_StopTextInput();
                	nameInput = true ;
				}
                
                if ( event.key.keysym.sym == SDLK_BACKSPACE && !name.empty() )
                    name.pop_back();
                
            }
            if (event.type == SDL_TEXTINPUT) 
                name += event.text.text;     
			if( name.length() >= 15 )
				name.pop_back() ;       
        }
    }
    
    if ( data.find(name) == data.end() ) 
        data[name] = std::vector<int>( levels.getTotalLevels( ) , 0 ) ;

    SDL_Delay( 14 ) ;
}

void Game::instructionsMenu()
{
    bool instructionsVisible = true ;
    SDL_Event event ; 
    int startTime = SDL_GetTicks() ;
    
    while ( instructionsVisible ) 
    {
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 255);
        SDL_RenderClear(renderer);
        
        DrawFont("Game Guidelines" , SCREEN_WIDTH / 3 + 30 , SCREEN_HEIGHT / 10 , 220 , 40 , 23 , { 128, 128 , 128 } ) ;
        SDL_Rect mouseSprite_rect = { SCREEN_WIDTH / 6 - 75 , SCREEN_HEIGHT / 5 - 15 , 150 , 150 };
        SDL_RenderCopy(renderer, textures["mouse"], nullptr, &mouseSprite_rect);
        DrawFont("Hold/Release Mouse", SCREEN_WIDTH / 6 - 80, SCREEN_HEIGHT / 2 - 20, 175 , 32, 24, { 0, 0, 0 });
        DrawFont("For Aim/Launch", SCREEN_WIDTH / 6 - 55, SCREEN_HEIGHT / 2 , 125, 32, 24, { 0, 0, 0 });

        SDL_Rect spaceSprite_rect = { SCREEN_WIDTH / 2 - 70 , SCREEN_HEIGHT / 3 , 120, 50 };
        SDL_RenderCopy(renderer, textures["space"], nullptr, &spaceSprite_rect);
        DrawFont("Press Space to Pause", SCREEN_WIDTH / 2 - 110 , SCREEN_HEIGHT / 2 - 20, 200, 32, 24, { 0, 0, 0 });

        SDL_Rect escapeSprite_rect = { ( 3 * SCREEN_WIDTH ) / 4 , SCREEN_HEIGHT / 3 - 15 , 100 , 80 };
        SDL_RenderCopy(renderer, textures["escape"], nullptr, &escapeSprite_rect);
        DrawFont("Press Escape to Exit Game", ( 3 * SCREEN_WIDTH ) / 4 - 70, SCREEN_HEIGHT / 2 - 20, 250, 32, 24, { 0, 0, 0 });

        SDL_Rect pKeySprite_rect = { SCREEN_WIDTH / 2 - 40 , SCREEN_HEIGHT / 2 + 35 , 64, 64 };
        SDL_RenderCopy(renderer, textures["p_key"], nullptr, &pKeySprite_rect );
        DrawFont("Press P to Mute/Unmute Music", SCREEN_WIDTH / 2 - 115, SCREEN_HEIGHT - 120 , 250 , 35 , 250 , { 0 , 0 , 0 } );


        int currentTime = SDL_GetTicks();
        int elapsedTime = currentTime - startTime;
        int remainingTime = 5000 - elapsedTime;
        
        if ( remainingTime <= 0 )
            instructionsVisible = false;
        
        int barWidth = ( remainingTime / 5000.0 ) * SCREEN_WIDTH ; // 5 seconds
        SDL_SetRenderDrawColor(renderer, 213, 128, 90 , 255) ;
        SDL_Rect loadingBar = { 0, SCREEN_HEIGHT - 20, barWidth, 20 } ;
        SDL_RenderFillRect(renderer, &loadingBar);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                instructionsVisible = false;
                quit = true;
            }
        }
        SDL_Delay(16);
    }
}

void Game::run() 
{
    while (!quit) 
	{
        SDL_Event e;
        bool mouse_released = false;
        bool mouse_pressed = false;

        while (SDL_PollEvent(&e))
        {
            if ((e.type == SDL_QUIT) || ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_ESCAPE)))
                quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && !titleScreen)
                Pause(e);
            if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p )
            {
                if ( Mix_PausedMusic() != 1 )
                    Mix_PauseMusic() ;      
                else
                    Mix_ResumeMusic() ;

            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_0)
            {
                Mix_HaltMusic() ;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) 
			{
                mouse_down = true;
                mouse_pressed = true;
            }
            if (e.type == SDL_MOUSEBUTTONUP) 
			{
                mouse_down = false;
                mouse_released = true;
            }
        }

        SDL_PumpEvents();
        SDL_GetMouseState(&mouse_x, &mouse_y);

        SDL_Delay(12);

        if (titleScreen) 
		{
            if (splashTimer > 120) 
			{
                if (!playedSelect) 
				{
                    Mix_PlayChannel(-1, sounds["select"], 0);
                    playedSelect = true;
                }
                
                GetUserName( e );
                renderLevels() ;
                levels.inputLevelMode( e , quit ) ;
                if (!quit)
                {
                    highscore = "BEST: " + std::to_string( data[name][ levels.getCurrentLevel() - 1 ] ) ; 
                    initializeLevelMode() ;

				    instructionsMenu() ;
                }
                Mix_PlayChannel(-1, sounds["select"], 0);
                titleScreen = false;
                mouseDownX = mouse_x;
                mouseDownY = mouse_y;
            }
            else {
                if (!playedSplash) 
				{
                    Mix_PlayChannel(-1, sounds["splash"], 0);
                    playedSplash = true;
                }

                SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
                SDL_RenderClear(renderer);

                DrawFont( "Platform_Prodigy", SCREEN_WIDTH / 2 - 105, SCREEN_HEIGHT / 2 + 3, 200, 40, 250, { 213, 128, 90 } ) ;
                DrawFont("Elevate Your Limits", SCREEN_WIDTH / 2 - 105, SCREEN_HEIGHT / 2 + 40, 200, 40, 200, { 213, 128, 90 } );

                SDL_Rect splashEggSprite_rect = { SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 16 - 23, 32, 32 };
                SDL_RenderCopy(renderer, textures["player"], nullptr, &splashEggSprite_rect);

                SDL_RenderPresent(renderer);

                splashTimer += 1;
            }
        }
        else 
		{
            if (playCoinFX) 
			{
                Mix_PlayChannel(-1, sounds["coin"], 0);
                playCoinFX = false;
            }

            if (mouse_pressed && player.isOnPlatform()) 
			{
                Mix_PlayChannel(-1, sounds["aim"], 0);
                mouseDownX = mouse_x;
                mouseDownY = mouse_y;
            }

            if (mouse_released && player.isOnPlatform()) 
			{
                if (firstTime) 
                    firstTime = false;
                
                else 
				{
                    Mix_PlayChannel(-1, sounds["launch"], 0);

                    if (player.isOnPlatform())
                        player.setY(player.getY() - 1);

                    int velocityX = mouse_x - mouseDownX;
                    int velocityY = mouse_y - mouseDownY;

                    player.setVelocity(static_cast<double>(velocityX) * .08, static_cast<double>(velocityY) * .08);
                }
            }
            checkPlayerCollision() ;
            player.updatePosition( gravity ) ;

            if (player.getY() > SCREEN_HEIGHT) 
			{
                Mix_PlayChannel(-1, sounds["death"], 0);
                resetGame();
            }
            for (int i = 0; i < 4; i++) 
                platforms[i].updatePosition( levels.getCurrentLevel() ) ;

            lavaY = SCREEN_HEIGHT - 43 - sin(timer) * 5;
            timer += 0.05;

            SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
            SDL_RenderClear(renderer);

            if (mouse_down && player.isOnPlatform()) 
			{
                SDL_SetRenderDrawColor(renderer, 178, 150, 125, 255);
                SDL_RenderDrawLine(
                    renderer,
                    player.getX() + (player.getWidth() / 2),
                    player.getY() + (player.getHeight() / 2),
                    mouse_x + (player.getX() - mouseDownX) + (player.getWidth() / 2),
                    mouse_y + (player.getY() - mouseDownY) + (player.getHeight() / 2)
                );
            }

            for (int i = 0; i < 4; i++) 
			{
                SDL_Rect platformSprite_rect = { static_cast<int>(platforms[i].getX()), static_cast<int>(platforms[i].getY()) , 100, 32 };
                SDL_RenderCopy(renderer, textures["platform"], nullptr, &platformSprite_rect);

                if (platforms[i].getHasCoin()) 
				{
                    SDL_Rect coinSprite_rect = { platforms[i].getCoinX(), platforms[i].getCoinY(), 24, 24 };
                    SDL_RenderCopy(renderer, textures["coin"], nullptr, &coinSprite_rect);
                }
            }

            SDL_Rect playerSprite_rect = { static_cast<int>(player.getX()), static_cast<int>(player.getY()), 32, 32 };
            SDL_RenderCopy(renderer, textures["player"], nullptr, &playerSprite_rect);

            SDL_Rect lavaSprite_rect = { 0, static_cast<int>(lavaY), 800, 48 };
            SDL_RenderCopy(renderer, textures["lava"], nullptr, &lavaSprite_rect);

            SDL_Rect scoreBoxSprite_rect = { 17, 17, 102, 70 };
            SDL_RenderCopy(renderer, textures["scoreBox"], nullptr, &scoreBoxSprite_rect);

            DrawFont(score, 28, 20, 75, 64, 64, { 0, 0, 0 });
            DrawFont(highscore, 17, 90, 74, 32, 32, { 0, 0, 0 });

            SDL_RenderPresent(renderer);
        }
    }
    if( !titleScreen )
        saveHighScoreData(); // If abruptly exited game while playing.
}
