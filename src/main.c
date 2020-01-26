#include <stdio.h>
#include <time.h>
#include <math.h>
#include <matrix.h>
#include "SDL2/SDL.h"
#include "config.h"
#include "sre/texturebuffer.h"
#include "sre/rasterizer.h"
#include "sre/pipeline.h"

SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

unsigned int _texWidth = 600;
unsigned int _texHeight = 600;
SDL_Texture* _texture = NULL;

SMOL_Matrix _perspectiveMatrix;
size_t _theVao;

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

static size_t frame = 0;
void vertexShader(size_t count, SR_VecUnion *attribs, SR_Vec4f *vPos)
{
    SMOL_Matrix p = (SMOL_Matrix){.nRows = 4, .nCols = 1, .fields = (double*)(&attribs[0].vec3f)};
    p.fields[3] = 1.0;
    //p.fields[0] += sin((frame++/50) * M_PI/180 );
    //p.fields[1] += cos((frame++/50) * M_PI/180 );
    //SMOL_MultiplyMat(&_perspectiveMatrix, &p);
    memcpy(vPos, p.fields, sizeof(double)*4);
    
    //free(p.fields);
}

void init ()
{
    init_window();
    SR_Init();
    SR_SetViewPort(_texWidth, _texHeight);

    SMOL_PerspectiveMatrix(&_perspectiveMatrix, 90, _texWidth/_texHeight, 1.0, 100);
    
    _theVao = SR_GenVertexArray();
    SR_BindVertexArray(_theVao);

    double vertices[] = {
        -0.5, -0.5, -1.0, 
	-0.5,  0.5, -3.0,
	 0.5, -0.5, -1.0,
 	 0.5,  0.5, -3.0
    };

    size_t indices[] = {0, 1, 2, 2, 1, 3};
    
    SR_SetBufferData(SR_VERTEX_BUFFER, vertices, sizeof(vertices));
    SR_SetBufferData(SR_INDEX_BUFFER, indices, sizeof(indices));

    SR_SetVertexAttributeCount(1);
    SR_SetVertexAttribute(0, 3, sizeof(double)*3, 0);

    SR_BindShader(SR_VERTEX_SHADER, &vertexShader);
}

int main ()
{
    init();
    
    SDL_Event event;
    unsigned long frame = 0;
    double runTime = 0;
    int isRunning = 1;
    
    // Main loop
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 currTime = SDL_GetPerformanceCounter();
    double deltaTime = 0;
    
    while (isRunning) {
	lastTime = currTime;
	currTime = SDL_GetPerformanceCounter();
        deltaTime = (currTime - lastTime) / (double)SDL_GetPerformanceFrequency();
	
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
	
	// Rendering
	SR_Clear(SR_COLOR_BUFFER_BIT | SR_DEPTH_BUFFER_BIT);
	SR_DrawArray(SR_TRIANGLES, 6, 0);
	
	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL,
			  &(SR_GetFrameBuffer().colorBuffer.values[0]), _texWidth * 4);
	SDL_RenderCopyEx(_renderer, _texture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(_renderer);
	
	frame++;
	runTime += deltaTime;
    }

    printf("\nNumber of frames: %lu\nTotal Runtime %f\n", frame, runTime);
    printf("Average FPS: %f\n\n", 1.0 / (runTime / frame));
    
    // Shutdown
    SMOL_Free(&_perspectiveMatrix);
    
    SR_Shutdown();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

