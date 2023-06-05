#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

//additional header files
#include "Vector2.h"
#include "Player.h"
#include "Obstacle.h"

bool Initialize();
void RunLoop();
void ShutDown();

void ProcessInput();
void UpdateGame();
void GenerateOutput();
void DisplayText();
void InitSDLText();
void StartingText();
void EndingText();
void ResetGame();

class Game {
    public:
        Game():mWindow(nullptr), mIsRunning(true) {
            mTicksCount = 0;
        }

        ~Game() { }

        bool Initialize();
        void RunLoop();
        void ShutDown();

    private:
        //used to track state of snake
        enum State {
            EActive,
            EPaused,
            EDead
        };

        struct Player {
            Vector2 pos;
            Vector2 vel;
            Vector2 accel;
            SDL_Rect rect;
            bool inAir;
        };

        struct Obstacle {
            Vector2 pos;
            SDL_Rect rect;
        };

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Uint32 mTicksCount;

        float thickness = 16.0f, deltaTime, obstacleTimer, obstacleTimerCap;
        const int GRAVITY = 50;

        int playerScore = 0, resetClicks = 0, collisionIndex;
        
        bool mIsRunning, gameOver = false, startGame = false;

        Player player;

        SDL_Rect playerRect = {0,0,32,32};
        Vector2 playerPos = Vector2(350,100);
        Vector2 instantiationPoint = Vector2(1000,700 - 32);

        std::vector<Obstacle> obstacles, shadowObstacles;
        std::string playerScoreStr;

        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
        void DisplayText();
        void InitSDLText();
        void StartingText();
        void EndingText();
        void ResetGame();
};

void Game::InitSDLText() {
    //initialize, else load error
    if(TTF_Init() == -1) {
        std::cout << "Failed to initialize text.Error: " << TTF_GetError() << std::endl;
    }
}

void Game::ResetGame() {
    //reset variables
    gameOver = false;
    startGame = false;
    resetClicks = 0;
    playerScore = 0;
    player.inAir = true;
    player.pos = Vector2(350,100);
    obstacles.clear();
    shadowObstacles.clear();
}

void Game::StartingText() {
    SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            0,
            0,
            0,
            0
        );

        SDL_RenderClear(mRenderer);

        //draw game scene
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            255,
            255,
            255,
            255
        );

        TTF_Font* ourFont;
        SDL_Surface * titleSurfaceText;	
        SDL_Surface * optionSurfaceText;
        SDL_Texture * titleTexture;
        SDL_Texture * optionTexture;

        SDL_Rect titleText = {340,100,300,200};
        SDL_Rect optionText = {250,400,500,100};

        //load font file and set size
        ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

        //confirm font loaded
        if(ourFont == nullptr) {
            std::cout << "Could not load font" << std::endl;
            exit(1);
        }

        SDL_Color textColor = {255,255,255,255};

        //pixels from text
        titleSurfaceText = TTF_RenderText_Solid(ourFont,"JUMP!",textColor);
        optionSurfaceText = TTF_RenderText_Solid(ourFont,"HIT SPACEBAR TO PLAY",textColor);

        //set up texture
        titleTexture = SDL_CreateTextureFromSurface(mRenderer,titleSurfaceText);
        optionTexture = SDL_CreateTextureFromSurface(mRenderer,optionSurfaceText);

        SDL_FreeSurface(titleSurfaceText);
        SDL_FreeSurface(optionSurfaceText);

        SDL_RenderCopy(mRenderer,titleTexture,NULL,&titleText);
        SDL_RenderCopy(mRenderer,optionTexture,NULL,&optionText);

        //destroy texture
        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(optionTexture);

        //close font
        TTF_CloseFont(ourFont);

        SDL_RenderPresent(mRenderer); //swap front and back buffers
}

void Game::EndingText() {
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            0,
            0,
            0,
            0
        );

        SDL_RenderClear(mRenderer);

        //draw game scene
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            255,
            255,
            255,
            255
        );
        TTF_Font* ourFont;
        SDL_Surface * titleSurfaceText;	
        SDL_Surface * optionSurfaceText;
        SDL_Texture * titleTexture;
        SDL_Texture * optionTexture;

        SDL_Rect titleText = {340,100,300,200};
        SDL_Rect optionText = {250,400,500,100};

        //load font file and set size
        ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

        //confirm font loaded
        if(ourFont == nullptr) {
            std::cout << "Could not load font" << std::endl;
            exit(1);
        }

        SDL_Color textColor = {255,255,255,255};

        //pixels from text
        titleSurfaceText = TTF_RenderText_Solid(ourFont,"JUMP!",textColor);
        optionSurfaceText = TTF_RenderText_Solid(ourFont,"EVERYONE WINS! A FOR EFFORT",textColor);

        //set up texture
        titleTexture = SDL_CreateTextureFromSurface(mRenderer,titleSurfaceText);
        optionTexture = SDL_CreateTextureFromSurface(mRenderer,optionSurfaceText);

        SDL_FreeSurface(titleSurfaceText);
        SDL_FreeSurface(optionSurfaceText);

        SDL_RenderCopy(mRenderer,titleTexture,NULL,&titleText);
        SDL_RenderCopy(mRenderer,optionTexture,NULL,&optionText);

        //destroy texture
        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(optionTexture);

        //close font
        TTF_CloseFont(ourFont);

        SDL_RenderPresent(mRenderer); //swap front and back buffers
        
}

void Game::DisplayText() {
    //add text
    TTF_Font* ourFont;
    SDL_Surface * playerSurfaceText;	
    SDL_Surface * AISurfaceText;
    SDL_Texture * playerTexture;
    SDL_Texture * AITexture;

    SDL_Rect playerScoreText = {950,25,50,50};

    //load font file and set size
    ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

    //confirm font loaded
    if(ourFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    SDL_Color textColor = {255,255,255,255};

    playerScoreStr = std::to_string(playerScore);
    char const *pchar = playerScoreStr.c_str();  //use char const* as target type

    //pixels from text
    playerSurfaceText = TTF_RenderText_Solid(ourFont,pchar,textColor);

    //set up texture
    playerTexture = SDL_CreateTextureFromSurface(mRenderer,playerSurfaceText);

    SDL_FreeSurface(playerSurfaceText);

    SDL_RenderCopy(mRenderer,playerTexture,NULL,&playerScoreText);

    //destroy texture
    SDL_DestroyTexture(playerTexture);

    //close font
    TTF_CloseFont(ourFont);

    SDL_RenderPresent(mRenderer); //swap front and back buffers
}

bool Game::Initialize() {
    //random number generator seed
    srand (time(NULL));

    //initialize SDL library
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    //Initialize text
    InitSDLText();

    if(sdlResult != 0) {
        //initialization failed
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //if initialization successful, create window
    mWindow = SDL_CreateWindow(
        "JUMP!",
        100,
        100,
        1024,
        768,
        0 // Flags
    );

    if(!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //initialization flags. use accelerated renderer and enable vsync
    );

    //initialize player
    player.pos.x = 350;
    player.pos.y = 100;
    player.vel.x = 0;
    player.vel.y = 300;
    player.accel.x = 350;
    player.accel.y = 1350;

    player.rect.x = (int)player.pos.x;
    player.rect.y = (int)player.pos.y;
    player.rect.h = 32;
    player.rect.w = 32;

    obstacleTimerCap = 1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(5.0f-1.0f)));
    //std::cout << obstacleTimerCap << std::endl;

    //if window and initialization successful, return true
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ShutDown() {
    //destroy SDL_Window
    SDL_DestroyWindow(mWindow);

    //destroy SDL_Renderer
    SDL_DestroyRenderer(mRenderer);

    //close SDL
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;

    //go through all events and respond as desired/appropriate
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                    case SDLK_SPACE:
                        if(!gameOver && !startGame && resetClicks < 1) {
                            resetClicks++;
                            player.inAir = true;
                        }
                        else if(gameOver && resetClicks == 2) {
                            ResetGame();
                        }
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Retrieve the state of all of the keys then scan each as desired
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }

    /* player.vel.x = 0;
    if(state[SDL_SCANCODE_RIGHT]) {
        player.vel.x += player.accel.x;
    } else if(state[SDL_SCANCODE_LEFT]) {
        player.vel.x -= player.accel.x;
    } */

    if(resetClicks < 1) { return; }

    if(!startGame) { 
        startGame = true; 
        return;
    }
    
    if((state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_UP]) && !player.inAir) {
        player.vel.y = -player.accel.y;
        player.inAir = true;
    }
}

void Game::UpdateGame() {
    //update frame at fixed intervals (fixedDeltaTime)
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    //get deltaTime
    deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    //clamp deltaTime
    if(deltaTime > .05f) {
        deltaTime = 0.05f;
    }

    if(!startGame || gameOver) { return; }

    //obstacle management
    obstacleTimer += deltaTime;
    if(obstacleTimer > obstacleTimerCap) {
        obstacleTimerCap = 1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(5.0f-1.0f)));
        //std::cout << obstacleTimerCap << std::endl;
        float difference = 1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.25f-1.0f)));
        
        //instantiate obstacle
        Obstacle obstacle;
        obstacle.pos.x = instantiationPoint.x;
        obstacle.pos.y = instantiationPoint.y - 32 * difference;

        obstacle.rect.x = (int)obstacle.pos.x;
        obstacle.rect.y = (int)obstacle.pos.y;
        obstacle.rect.h = 32 + 32 * difference;
        obstacle.rect.w = 32;

        Obstacle shadowObstacle;
        shadowObstacle.pos.x = 1000;
        shadowObstacle.pos.y = 0;

        shadowObstacle.rect.x = (int)shadowObstacle.pos.x;
        shadowObstacle.rect.y = (int)shadowObstacle.pos.y;
        shadowObstacle.rect.h = 800;
        shadowObstacle.rect.w = 32;

        obstacles.emplace_back(obstacle);
        shadowObstacles.emplace_back(shadowObstacle);

        obstacleTimer = 0.0f;
    }

    //update obstacle(s) position(s)
    if(shadowObstacles.size() > 0) {
        for(int i = 0; i < shadowObstacles.size(); i++) {
            //obstacles[i].rect.x -= 350 * deltaTime;
            shadowObstacles[i].rect.x -= 350 * deltaTime;
        }
    }

    if(obstacles.size() > 0) {
        for(int i = 0; i < obstacles.size(); i++) {
            obstacles[i].rect.x -= 350 * deltaTime;
        }
    }

    if(obstacles.size() > 0) {
        if(obstacles[0].rect.x < 0) {
            //std::cout << "ERASED!" << std::endl;
            obstacles.erase(obstacles.begin());
        }
    }

    //update player position
    player.vel.y += GRAVITY;

    //player.pos.x += player.vel.x * deltaTime;
    player.pos.y += player.vel.y * deltaTime;

    //platform collision detection
    if(player.pos.y + player.rect.h >= 700) {
        player.pos.y = 700 - player.rect.h;
        player.vel.y = 0;
        player.inAir = false;
    }

    int removeAtIndex = shadowObstacles.size() + 1;

    //player score tracking
    for(int i = 0; i < shadowObstacles.size(); i++) {
        SDL_bool collision = SDL_HasIntersection(&shadowObstacles[i].rect, &player.rect);
        if(collision && removeAtIndex == shadowObstacles.size() + 1) { 
            playerScore++;
            removeAtIndex = i;
        }
    }
    
    if(removeAtIndex < shadowObstacles.size() + 1) {
        shadowObstacles.erase(shadowObstacles.begin() + removeAtIndex);
    }
    

    //player obstacle collision detection
    for(int i = 0; i < obstacles.size(); i++) {
        SDL_bool collision = SDL_HasIntersection(&obstacles[i].rect, &player.rect);
        if(collision) { 
            resetClicks++;
            gameOver = true; 
        }
    }

    //player.rect.x = (int)player.pos.x;
    player.rect.y = (int)player.pos.y;
}

void Game::GenerateOutput() {

    if(!gameOver && !startGame && resetClicks == 0) { //game not started yet 
        StartingText();
        return;
    } else if(gameOver) { //game played till end
        if(resetClicks == 2) {
            EndingText();
            return;
        }
    }

    SDL_SetRenderDrawColor(mRenderer,0,0,0,0);

    SDL_RenderClear(mRenderer); //clear back buffer to current draw color

    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);

    SDL_Rect topWall = {0,0,1024,thickness};
    SDL_Rect bottomWall = {0,768 - 4*thickness,1024,4*thickness};
    SDL_Rect leftWall = {0,0,thickness,768};
    SDL_Rect rightWall = {1008,0,thickness,768};

    SDL_RenderFillRect(mRenderer,&topWall);
    SDL_RenderFillRect(mRenderer,&rightWall);
    SDL_RenderFillRect(mRenderer,&leftWall);
    SDL_RenderFillRect(mRenderer,&bottomWall);

    if(obstacles.size() > 0) {
        for(int i = 0; i < obstacles.size(); i++) {
            SDL_RenderFillRect(mRenderer,&obstacles[i].rect);
        }
    }

    SDL_SetRenderDrawColor(mRenderer,255,0,0,255);
    SDL_RenderFillRect(mRenderer,&player.rect);

    DisplayText();
} 
