#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "Game.h"

int main(int argc, char** argv) {
    Game game;
    bool success = game.Initialize();

    if(success) {
        game.RunLoop();
    }

    game.ShutDown();
    return 0;
}
