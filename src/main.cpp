#include <cstdio>
#include <cstdlib>

#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "config.h"

SDL_Window *_window = NULL;
SDL_GLContext _glContext = NULL;

static void sdl_die(const char * message) {
    // Print Error Message and Die
    fprintf(stderr, "%s: %s.\n", message, SDL_GetError());
    exit(2);
}

void resize_window() {
    int w, h;
    SDL_GetWindowSize(_window, &w, &h);
    glViewport(0, 0, w, h);
}

void init_window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	sdl_die("Error initializing SDL");

    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    _window = SDL_CreateWindow(
        PROJECT_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	800, 600,
	SDL_WINDOW_OPENGL);

    if (_window == NULL)
	sdl_die("Error setting video mode");

    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == NULL) {
	sdl_die("Failed to create OpenGL context");
    }

    printf("OpenGL loaded\n");
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    SDL_GL_SetSwapInterval(1);

    resize_window();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glClearColor(1.0, 0.0, 0.0, 1.0);
}

int main () {
    init_window();

    SDL_Event event;
    bool quit = false;
    while (!quit) {
	SDL_GL_SwapWindow(_window);
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) 
		quit = true;
	}
    }
    
    SDL_DestroyWindow(_window);
    SDL_Quit();
    
    return 0;
}

