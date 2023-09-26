#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "../vendor/SDL2/SDL.h"
#include "../vendor/stb_image/stb_image.h"
#include "Math.h"
#include <stdio.h>
#include <iostream> 

#include "Application.h"
#include "Rasterizer.h"

constexpr int SCREEN_WIDTH = 1920 / 2;
constexpr int SCREEN_HEIGHT = 1080 / 2;

int main(int argv, char* args[]) {
	int w;
	int h;
	int comp;
	// stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load("wallpaper0.jpg", &w, &h, &comp, STBI_rgb_alpha);
	Backbuffer imageBuffer;
	imageBuffer.basePixel = reinterpret_cast<uint32_t*>(image);
	imageBuffer.width = (uint32_t)w;
	imageBuffer.height = (uint32_t)h;

	Screen screen{init("title", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)};

	Backbuffer backbuffer{};
	backbuffer.basePixel = static_cast<uint32_t*>(screen.surface->pixels);
	backbuffer.height = SCREEN_HEIGHT;
	backbuffer.width = SCREEN_WIDTH;

	Depthbuffer depthbuffer{};
	depthbuffer.basePixel = (float*)SDL_malloc(sizeof(float) * backbuffer.height * backbuffer.width);
	depthbuffer.width = backbuffer.width;
	depthbuffer.height = backbuffer.height;

	bool running{true};
	Vertex triangles[2][3];
	SDL_Event e;

	uint32_t tick{SDL_GetTicks()};
	uint32_t ogTick{tick};
	while(running) {
		ogTick = tick;
		tick = SDL_GetTicks();
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN: {
					SDL_Scancode scan{e.key.keysym.scancode};
					if (scan == SDL_SCANCODE_TAB) {
						running = false;
					}	
					if (scan == SDL_SCANCODE_SPACE) {
						tick = ogTick;
					}
				} break;
				default:
					break;
			}
		}
		
		float radius{2.f};
		float speed{0.001};
		Mat4x4 mat{};
		
		// rot
		mat.v0.x = radius * cosf(tick * speed);
		mat.v0.y = radius * sinf(tick * speed) * -1;

		mat.v1.x = radius * sinf(tick * speed);
		mat.v1.y =  radius * cosf(tick * speed);

		// trans
		mat.v0.w = 200;
		mat.v1.w = 200;
		mat.v2.w = 1;
		mat.v3.w = 1;

		Vec4 p0{};
		Vec4 p1{};
		Vec4 p2{};
		Vec4 p3{};
		Vec4 p4{};

		p0.w = 1.f;
		p1.w = 1.f;
		p2.w = 1.f;
		p3.w = 1.f;

		p0.x = -100.f;
		p0.y = -100.f;

		p1.x = 100.f;
		p1.y = -100.f;

		p2.x = 100.f;
		p2.y = 100.f;
		p2.z = 0.9f;

		float sP2x = 300.f;
		float sP2y = 200.f;
		float sP2z = 0.f;

		p3.x = -100.f;
		p3.y = 100.f;

		p0 = mat * p0;
		p1 = mat * p1;
		p2 = mat * p2;
		p3 = mat * p3;

		triangles[0][0].p = {p0.x, p0.y, p0.z};
		triangles[0][1].p = {p2.x, p2.y, p2.z};
		triangles[0][2].p = {p1.x, p1.y, p1.z};

		triangles[1][0].p = {p0.x, p0.y, p0.z};
		triangles[1][1].p = {p3.x, p3.y, p3.z};
		triangles[1][2].p = {sP2x, sP2y, sP2z};

		triangles[0][0].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		triangles[0][1].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		triangles[0][2].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		triangles[1][0].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		triangles[1][1].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		triangles[1][2].p.x *= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

		triangles[0][0].c = {1.0f, 0.0f, 0.0f, 1.0f};
		triangles[0][1].c = {0.0f, 1.0f, 0.0f, 1.0f};
		triangles[0][2].c = {0.0f, 0.0f, 1.0f, 1.0f};

		triangles[1][0].c = {1.0f, 0.0f, 0.0f, 1.0f};
		triangles[1][1].c = {0.0f, 1.0f, 0.0f, 1.0f};
		triangles[1][2].c = {0.0f, 0.0f, 1.0f, 1.0f};

		triangles[0][0].uv = {0, 0};
		triangles[0][1].uv = {1, 1};
		triangles[0][2].uv = {1, 0};

		triangles[1][0].uv = {0, 0};
		triangles[1][1].uv = {0, 1};
		triangles[1][2].uv = {1, 1};

		// SDL_FillRect(screen.surface, 0, SDL_MapRGB(screen.surface->format, 255, 0, 255));

		SDL_LockSurface(screen.surface);
		clearBuffer(backbuffer, 1.0f, 0, 1.0f);
		clearBuffer(depthbuffer);
		rasterizeTriangleImage(backbuffer, imageBuffer, depthbuffer, triangles[0]);
		rasterizeTriangleImage(backbuffer, imageBuffer, depthbuffer, triangles[1]);
		// rasterizeTriangle(backbuffer, triangles[0]);
		// rasterizeTriangle(backbuffer, triangles[1]);
		SDL_UnlockSurface(screen.surface);

    	SDL_UpdateWindowSurface(screen.window);
	}

	SDL_DestroyWindowSurface(screen.window);
    SDL_DestroyWindow( screen.window );

    //Quit SDL subsystems
    SDL_Quit();
	stbi_image_free(image);

    return 0;
}
