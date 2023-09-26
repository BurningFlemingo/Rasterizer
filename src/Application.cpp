#include "Application.h"

Screen init(const char* title, uint32_t windowWidth, uint32_t windowHeight, uint32_t flags) {
	Screen screen{};
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return {};
    }

    screen.window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED,
			   SDL_WINDOWPOS_UNDEFINED, windowWidth,
			   windowHeight, flags );
    if(!screen.window) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return {};
    }

    screen.surface = SDL_GetWindowSurface(screen.window);
	screen.context = SDL_GL_CreateContext(screen.window);
    if(!screen.surface) {
        printf( "Surface could not be created! SDL_Error: %s\n", SDL_GetError() );
		return {};
    }
    if(!screen.context) {
        printf( "Context could not be created! SDL_Error: %s\n", SDL_GetError() );
		return {};
    }

	return screen;
}
