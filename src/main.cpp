#include <cstdio>
#include <cstdlib>
#include <vector>
#include <glm/vec4.hpp>
#include "SDL2/SDL.h"
#include "config.h"
#include "pixelbuffer.h"

SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

unsigned int _texWidth = 600;
unsigned int _texHeight = 600;
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
    bool isRunning = true;
    
    // Main loop
    while (isRunning) {
	const Uint64 start = SDL_GetPerformanceCounter();
	
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
		isRunning = false;
		break;
	    }
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		    isRunning = false;
		    break;
		}
	    }
	}

	ColorBuffer *buffer = CreateColorBuffer(_texWidth, _texHeight);

	uint32_t cx = 300, cy = 300;
	WriteLine(buffer, cx, cy, 600, 600, 255);
	WriteLine(buffer, cx, cy, 600, 550, 225);
	WriteLine(buffer, cx, cy, 600, 500, 195);
	WriteLine(buffer, cx, cy, 600, 450, 175);
	WriteLine(buffer, cx, cy, 600, 400, 155);
	WriteLine(buffer, cx, cy, 600, 350, 135);
	WriteLine(buffer, cx, cy, 600, 300, 105);

	WriteLine(buffer, cx, cy, 550, 600, 255);
	WriteLine(buffer, cx, cy, 500, 600, 225);
	WriteLine(buffer, cx, cy, 450, 600, 195);
	WriteLine(buffer, cx, cy, 400, 600, 175);
	WriteLine(buffer, cx, cy, 350, 600, 155);
	WriteLine(buffer, cx, cy, 300, 600, 135);

	WriteLine(buffer, cx, cy, 0, 0, 255);
	WriteLine(buffer, cx, cy, 0, 50, 225);
	WriteLine(buffer, cx, cy, 0, 100, 195);
	WriteLine(buffer, cx, cy, 0, 150, 175);
	WriteLine(buffer, cx, cy, 0, 200, 155);
	WriteLine(buffer, cx, cy, 0, 250, 135);
	WriteLine(buffer, cx, cy, 0, 300, 105);

	WriteLine(buffer, cx, cy, 50, 0, 225);
	WriteLine(buffer, cx, cy, 100, 0, 195);
	WriteLine(buffer, cx, cy, 150, 0, 175);
	WriteLine(buffer, cx, cy, 200, 0, 155);
	WriteLine(buffer, cx, cy, 250, 0, 135);
	//WriteLine(buffer, cx, cy, 300, 0, 105);

	
	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL, &(buffer->buffer[0]), _texWidth * 4);
	SDL_RenderCopy(_renderer, _texture, NULL, NULL );
        SDL_RenderPresent(_renderer);

	delete buffer;
	
	// Performance measurements
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
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

