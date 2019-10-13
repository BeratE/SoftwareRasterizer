#include <iostream>
#include "SDL2/SDL.h"

int main () {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "Real-Time Graphics",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	800, 600,
	0);

    std::cout << "Hello World" << std::endl;

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

