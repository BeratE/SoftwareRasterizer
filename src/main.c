#include <stdio.h>
#include "SDL2/SDL.h"

#include "config.h"
#include "colorbuffer.h"
#include "rasterizer.h"


SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

uint_fast32_t _texWidth = 600;
uint_fast32_t _texHeight = 600;
SDL_Texture* _texture = NULL;


static void sdl_die(const char * message)
/* Print Error Message and Die. */
{
    fprintf(stderr, "%s: %s.\n", message, SDL_GetError());
    exit(2);
}

void init_window()
/* Initialize the window display and screen texture as well as the required renderer. 
 * Initialize basic states and configurations of said resources. */    
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	sdl_die("Error initializing SDL");
    atexit(SDL_Quit);

    _window = SDL_CreateWindow(
        PROJECT_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	1024, 1024,
	SDL_WINDOW_SHOWN);

    if (_window == NULL)
	sdl_die("Error setting video mode");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_RendererInfo info;
    SDL_GetRendererInfo(_renderer, &info);
    printf("Renderer: %s\n", info.name);
    printf("Texture formats:\n");
    for (unsigned int i = 0; i < info.num_texture_formats; i++)
	printf("%s\n", SDL_GetPixelFormatName(info.texture_formats[i]));

    _texture = SDL_CreateTexture(_renderer,
				 SDL_PIXELFORMAT_ABGR8888,
				 SDL_TEXTUREACCESS_STREAMING, _texWidth, _texHeight);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

int main ()
{
    init_window();
    
    SDL_Event event;
    uint64_t frame = 0;
    double runTime = 0;
    int isRunning = 1;
    ColorBuffer *buffer = CreateColorBuffer(_texWidth, _texHeight, 4);
    
    // Main loop
    while (isRunning) {
	const Uint64 start = SDL_GetPerformanceCounter();
	
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
		isRunning = 0;
		break;
	    }
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		    isRunning = 0;
		    break;
		}
	    }
	}

	ClearColorBuffer(buffer, 0);
	
	uint32_t cx = 300, cy = 300;
	for (int i = 0; i <= 300; i += 20) {
	    WriteLine(buffer, cx, cy, 600, 600 - i, 255);
	    WriteLine(buffer, cx, cy, 600 - i, 600, 160);

	    WriteLine(buffer, cx, cy, 0, 300 - i, 255);
	    WriteLine(buffer, cx, cy, 300 - i, 0, 160);
	}
	
	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL, &(buffer->array[0]), _texWidth * 4);
	SDL_RenderCopy(_renderer, _texture, NULL, NULL );
        SDL_RenderPresent(_renderer);
	
	// Performance measurements
        const Uint64 end = SDL_GetPerformanceCounter();
        const Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = (end - start) / (double)(freq);
	frame++;
	runTime += seconds;
    }

    printf("\nNumber of frames: %lu\nTotal Runtime %f\n", frame, runTime);
    printf("Average FPS: %f\n\n", 1.0 / (runTime / frame));
    
    // Shutdown
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

