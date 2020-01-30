#include <stdio.h>
#include <time.h>
#include <math.h>
#include <matrix.h>
#include "SDL2/SDL.h"
#include "config.h"
#include "sre/texturebuffer.h"
#include "sre/rasterizer.h"
#include "sre/pipeline.h"

/* Global state */
SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

unsigned int _texWidth = 1600;
unsigned int _texHeight = 1600;
SDL_Texture* _texture = NULL;

SMOL_Matrix _perspectiveMat;
SMOL_Matrix _viewMat;
SMOL_Matrix _modelMat;
size_t _frame = 0;

SR_Pipeline _pipeline;
size_t _theVao;


Uint64 _lastTime, _currTime;
/* ~/Global state/~ */

/* Function definitions */
void vertexShader(size_t count, SR_Vecf *attribs, SR_Vec4f *vPos);
void fragmentShader(size_t count, SR_Vecf *attribs, SR_Vec4f *oColor);
/* ~/Function definitions/~ */

/* Shader functions */
void vertexShader(size_t count, SR_Vecf *attribs, SR_Vec4f *vPos)
{
    double t = _currTime/500000000.0;
    
    SMOL_Matrix p = (SMOL_Matrix){.nRows = 4, .nCols = 1, .fields = (double*)(&attribs[0].vec3f)};
    p.fields[3] = 1.0;

    SMOL_Scale(&p, 1.0+(0.5*cos(t)));
    p.fields[3] = 1.0;
    
    SMOL_Matrix rot;
    SMOL_RotationMatrix(&rot, (double[]){sin(t), 0.0, cos(t)}, (t)*(M_PI/180));
    /*
    SMOL_Free(&_viewMat);
    SMOL_CameraMatrix(&_viewMat,
		      (double[]){5.0*cos(t), 0.0, 15.0+5.0*sin(t)},
		      (double[]){0.0, 0.0, 0.0},
		      (double[]){0.0, 1.0, 0.0});
    */
    SMOL_Matrix mat;
    SMOL_Matrix eye;
    SMOL_EyeMatrix(&eye, 4);
    SMOL_MultiplyV(&mat, 3, &rot, &_viewMat, &_perspectiveMat);
    
    SMOL_Matrix k;
    SMOL_Multiply(&k, &mat, &p);
    memcpy(vPos, k.fields, sizeof(double)*4);

    SMOL_FreeV(1, &k);
    SMOL_FreeV(2, &mat, &eye);
}

void fragmentShader(size_t count, SR_Vecf *attribs, SR_Vec4f *fColor)
{
}
/* ~/Shader functions/~ */


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

void init ()
{
    init_window();
    SR_Init();
    SR_SetViewPort(_texWidth, _texHeight);

    SMOL_PerspectiveMatrix(&_perspectiveMat, 90, _texWidth/_texHeight, 0.1, 100);
    SMOL_CameraMatrix(&_viewMat, (double[]){0.0, 0.0, 10.0}, (double[]){0.0, 0.0, 0.0}, (double[]){1.0, 1.0, 0.0});
    SMOL_EyeMatrix(&_modelMat, 4);
    
    _theVao = SR_GenVertexArray();
    SR_BindVertexArray(_theVao);

     double vertices[] = {
	     // front
	     -1.0, -1.0,  1.0,
	     1.0, -1.0,  1.0,
	     1.0,  1.0,  1.0,
	     -1.0,  1.0,  1.0,
	     // back
	     -1.0, -1.0, -1.0,
	     1.0, -1.0, -1.0,
	     1.0,  1.0, -1.0,
	     -1.0,  1.0, -1.0
     };


     size_t indices[] = {
	     // front
	     0, 1, 2,
	     2, 3, 0,
	     // right
	     1, 5, 6,
	     6, 2, 1,
	     // back
	     7, 6, 5,
	     5, 4, 7,
	     // left
	     4, 0, 3,
	     3, 7, 4,
	     // bottom
	     4, 5, 1,
	     1, 0, 4,
	     // top
	     3, 2, 6,
	     6, 7, 3
     };
    
    SR_SetBufferData(SR_VERTEX_BUFFER, vertices, sizeof(vertices));
    SR_SetBufferData(SR_INDEX_BUFFER, indices, sizeof(indices));

    SR_SetVertexAttributeCount(1);
    SR_SetVertexAttribute(0, 3, sizeof(double)*3, 0);

    _pipeline.vertexShader = &vertexShader;
    _pipeline.fragmentShader = &fragmentShader;
    SR_BindPipeline(&_pipeline);
}

int main ()
{
    init();
    
    SDL_Event event;
    double runTime = 0;
    int isRunning = 1;
    
    // Main loop
    _lastTime = SDL_GetPerformanceCounter();
    _currTime = SDL_GetPerformanceCounter();
    double deltaTime = 0;
    
    while (isRunning) {
	_lastTime = _currTime;
	_currTime = SDL_GetPerformanceCounter();
        deltaTime = (_currTime - _lastTime) / (double)SDL_GetPerformanceFrequency();
	
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
	SR_DrawArray(SR_TRIANGLES, 36, 0);
	
	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL,
			  &(SR_GetFrameBuffer().colorBuffer.values[0]), _texWidth * 4);
	SDL_RenderCopyEx(_renderer, _texture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(_renderer);
	
	_frame++;
	runTime += deltaTime;
    }

    printf("\nNumber of frames: %lu\nTotal Runtime %f\n", _frame, runTime);
    printf("Average FPS: %f\n\n", 1.0 / (runTime / _frame));
    
    // Shutdown
    SMOL_FreeV(2, &_perspectiveMat, &_viewMat);
    
    SR_Shutdown();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

