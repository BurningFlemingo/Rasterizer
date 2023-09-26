#pragma once

#include "glad/glad.h"
#include "../vendor/SDL2/SDL.h"

struct Screen {
	SDL_Surface* surface;
	SDL_Window* window;
	SDL_GLContext context;
};

Screen init(const char* title, uint32_t windowWidth, uint32_t windowHeight, uint32_t flags);
