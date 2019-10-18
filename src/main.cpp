#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL2/SDL.h"
#include "config.h"

SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

unsigned int _texWidth = 1024;
unsigned int _texHeight = 1024;
SDL_Texture* _texture = NULL;

static void sdl_die(const char * message)
/* Print Error Message and Die. */
{
    fprintf(stderr, "%s: %s.\n", message, SDL_GetError());
    exit(2);
}

void resize_window()
{
    int w, h;
    SDL_GetWindowSize(_window, &w, &h);
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
	800, 600,
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
    bool quit = false;
    // Main loop
    while (!quit) {
	const Uint64 start = SDL_GetPerformanceCounter();
	
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
		quit = true;
		break;
	    }
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		    quit = true;
		    break;
		}
	    }
	}

	std::vector<uint8_t> pixels(_texWidth * _texHeight * 4, 0);
	
	for( unsigned int i = 0; i < 1000; i++ )
        {
            const unsigned int x = rand() % _texWidth;
            const unsigned int y = rand() % _texHeight;

            const unsigned int offset = (_texWidth * 4 * y ) + x * 4;
            pixels[ offset + 0 ] = rand() % 256;        // b
            pixels[ offset + 1 ] = rand() % 256;        // g
            pixels[ offset + 2 ] = rand() % 256;        // r
            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        }

	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL, &pixels[0], _texWidth * 4);
	SDL_RenderCopy(_renderer, _texture, NULL, NULL );
        SDL_RenderPresent(_renderer);

	// Performance measurements
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        printf("Frame time: %f ms\n", seconds * 1000.0);
    }

    // Shutdown
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

