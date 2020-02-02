#include <stdio.h>
#include <time.h>
#include <math.h>
#include <matrix.h>
#include "SDL2/SDL.h"
#include "config.h"
#include "sre/sre.h"

/* Global state */
SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;

unsigned int _texWidth = 800;
unsigned int _texHeight = 800;
SDL_Texture* _texture = NULL;

const int NUM_CUBES = 1;
SMOL_Matrix _cubeMats[1];
size_t _frame = 0;
double _runTime = 0;
Uint64 _lastTime, _currTime;

SMOL_Matrix _perspectiveMat;
SMOL_Matrix _viewMat;
SMOL_Matrix _modelMat;

size_t _theVao;
/* ~/Global state/~ */

/* Function definitions */
void vertexShader(size_t count, SR_Vecf *attribs, SR_Vec4f *vPos);
void fragmentShader(size_t count, SR_Vecf *attribs, SR_Vec4f *oColor);
/* ~/Function definitions/~ */

/* Shader functions */
void vertexShader(size_t count, SR_Vecf *attribs, SR_Vec4f *vPos)
{
    SR_Vec3f aPos = attribs[0].vec3f;
    SR_Vec3f aColor = attribs[1].vec3f;
    
    double t = _currTime/500000000.0;
    
    SMOL_Matrix p = (SMOL_Matrix){.nRows = 4, .nCols = 1, .fields = (double*)&aPos};
    p.fields[3] = 1.0;
   
    SMOL_Matrix mat;
    SMOL_MultiplyV(&mat, 3, &_modelMat, &_viewMat, &_perspectiveMat);
    
    SMOL_Matrix k;
    SMOL_Multiply(&k, &mat, &p);
    memcpy(vPos, k.fields, sizeof(double)*4);

    SR_SetVertexStageOutput(0, &aColor);

    SMOL_FreeV(2, &k, &mat);
}

void fragmentShader(size_t count, SR_Vecf *attribs, SR_Vec4f *fColor)
{
    *fColor = attribs[0].vec4f;
    fColor->w = 1.0;
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
    SMOL_CameraMatrix(&_viewMat,
		      (double[]){0.0, 0.0, 5.0},
		      (double[]){0.0, 0.0, 0.0},
		      (double[]){0.0, 1.0, 0.0});

    for (int i = 0; i < NUM_CUBES; i++) {
	SMOL_EyeMatrix(&_cubeMats[i], 4);
	/* SMOL_SetField(&_cubeMats[i], 0, 3, (rand()%10) * ((rand()%2) ? 1.0 : -1.0)); */
	/* SMOL_SetField(&_cubeMats[i], 1, 3, (rand()%10) * ((rand()%2) ? 1.0 : -1.0)); */
	/* SMOL_SetField(&_cubeMats[i], 2, 3, (rand()%20)); */
    }
    
    _theVao = SR_GenVertexArray();
    SR_BindVertexArray(_theVao);
  
     double vertices[] = {
	 // front          // color
	 -1.0, -1.0,  1.0, 1.0, 0.25, 0.5,
	  1.0, -1.0,  1.0, 1.0, 0.45, 0.0,
	  1.0,  1.0,  1.0, 1.0, 0.75, 0.5,
	 -1.0,  1.0,  1.0, 1.0, 0.10, 0.0,
	 // back
	 -1.0, -1.0, -1.0, 0.0, 0.10, 0.5,
	  1.0, -1.0, -1.0, 0.0, 0.50, 0.25,
	  1.0,  1.0, -1.0, 0.5, 0.75, 0.25,
	 -1.0,  1.0, -1.0, 0.5, 0.55, 1.0
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

    SR_SetVertexAttributeCount(2);
    SR_SetVertexAttribute(0, 3, sizeof(double)*6, 0);
    SR_SetVertexAttribute(1, 3, sizeof(double)*6, sizeof(double)*3);

    SR_BindShader(SR_VERTEX_SHADER, &vertexShader);
    SR_BindShader(SR_FRAGMENT_SHADER, &fragmentShader);

    SR_SetVertexStageOutputCount(1);
}

int main ()
{
    init();
    
    SDL_Event event;
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

	double t = _currTime/500000000.0;
	for (int i = 0; i < NUM_CUBES; i++) {
	    SMOL_Matrix rot;
	    SMOL_RotationMatrix(&rot, (double[]){sin(t), 0.0, cos(t)}, (t)*(M_PI/180));

	    SMOL_Multiply(&_modelMat, &_cubeMats[i], &rot);
	    //_modelMat = _cubeMats[i];
	    SR_DrawArray(SR_TRIANGLES, 36, 0);

	    SMOL_FreeV(2, &rot, &_modelMat);
	}
	
	// Blit texture content to the screen
	SDL_UpdateTexture(_texture, NULL,
			  &(SR_GetFrameBuffer().color.values[0]), _texWidth * 4);
	SDL_RenderCopyEx(_renderer, _texture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(_renderer);
	
	_frame++;
	_runTime += deltaTime;
    }

    printf("\nNumber of frames: %lu\nTotal Runtime %f\n", _frame, _runTime);
    printf("Average FPS: %f\n\n", 1.0 / (_runTime / _frame));
    
    // Shutdown

    // Free matrices
    SMOL_FreeV(2, &_perspectiveMat, &_viewMat);
    for (int i = 0; i < NUM_CUBES; i++) {
	SMOL_Free(&_cubeMats[i]);
    }
    
    SR_Shutdown();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

