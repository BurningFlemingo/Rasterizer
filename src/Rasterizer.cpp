#include "Rasterizer.h"
#include "Math.h"
#include <iostream>
#include <assert.h>

#define PNG_RED_SHIFT 16
#define PNG_GREEN_SHIFT 8 
#define PNG_BLUE_SHIFT 0
#define PNG_ALPHA_SHIFT 24 

#define SCREEN_RED_SHIFT 0
#define SCREEN_GREEN_SHIFT 8 
#define SCREEN_BLUE_SHIFT 16
#define SCREEN_ALPHA_SHIFT 24

enum class ColorFormat : uint32_t {
	PNG, 
	SCREEN
};

uint32_t rgbaToColor(ColorFormat flag, float r, float g, float b, float a) {
	assert(r <= 1.f);
	assert(g <= 1.f);
	assert(b <= 1.f);
	assert(a <= 1.f);

	uint32_t red{(uint32_t)(r * 255.f)};
	uint32_t green{(uint32_t)(g * 255.f)};
	uint32_t blue{(uint32_t)(b * 255.f)};
	uint32_t alpha{(uint32_t)(a * 255.f)};

	uint32_t color{};
	switch(flag) {
		case ColorFormat::PNG:
			color = (red << PNG_RED_SHIFT | green << PNG_GREEN_SHIFT | blue << PNG_BLUE_SHIFT | alpha << PNG_ALPHA_SHIFT);
			break;
		case ColorFormat::SCREEN:
			color = (red << SCREEN_RED_SHIFT | green << SCREEN_GREEN_SHIFT | blue << SCREEN_BLUE_SHIFT | alpha << SCREEN_ALPHA_SHIFT);
			break;
		default:
			break;
	}

	return color;
}

void clearBuffer(Backbuffer backbuffer, float r, float g, float b) {
	for (uint32_t i{}; i < backbuffer.width * backbuffer.height; i++) {
		backbuffer.basePixel[i] = rgbaToColor(ColorFormat::SCREEN, r, g, b, 1.f);
	}
}

void clearBuffer(Depthbuffer dBuffer) {
	for (uint32_t i{}; i < dBuffer.width * dBuffer.height; i++) {
		dBuffer.basePixel[i] = INFINITY;
	}
}

float edgeFunction(Vec2 a, Vec2 b, Vec2 c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

constexpr uint32_t RENDER_WIDTH = 100;
constexpr uint32_t RENDER_HEIGHT = 100;

inline void drawPixel(Backbuffer* backbuffer, uint32_t x, uint32_t y, uint32_t color, uint32_t scale) {
	for (uint32_t i{}; i < scale; i++) {
		for (uint32_t j{}; j < scale; j++) {
			backbuffer->basePixel[((y * scale) + i) * backbuffer->width + ((x * scale) + j)] = color;
		}
	}
}

// points start at top left of screen
void rasterizeTriangle(Backbuffer backbuffer, Vertex points[3]) {
	int scale{1};
	Vec2 pointsVector[3] = {{points[0].p.x, points[0].p.y}, {points[1].p.x, points[1].p.y}, {points[2].p.x, points[2].p.y}};
	float triangleArea{edgeFunction(pointsVector[0], pointsVector[1], pointsVector[2])};

	for (int yPixel{}; yPixel < backbuffer.height; yPixel++) {
		for (int xPixel{}; xPixel < backbuffer.width; xPixel++) {
			int xRenderPixel{xPixel / scale};
			int yRenderPixel{yPixel / scale};

			Vec2 pixel{(float)xPixel / scale, (float)yPixel / scale};

			bool pointInside{true};

			float v0Weight{};
			float v1Weight{};
			float v2Weight{};

			v0Weight = edgeFunction(pointsVector[1], pointsVector[2], {(float)xRenderPixel + 0.5f, (float)yRenderPixel + 0.5f});
			v1Weight = edgeFunction(pointsVector[2], pointsVector[0], {(float)xRenderPixel + 0.5f, (float)yRenderPixel + 0.5f});
			v2Weight = edgeFunction(pointsVector[0], pointsVector[1], {(float)(xRenderPixel + 0.5f), (float)yRenderPixel + 0.5f});

			// Vec2 edge0{pointsVector[1] - pointsVector[0]};
			// Vec2 edge1{pointsVector[2] - pointsVector[1]};
			// Vec2 edge2{pointsVector[0] - pointsVector[2]};

			bool overlaps{true};

			overlaps &= v0Weight >= 0;
			overlaps &= v1Weight >= 0;
			overlaps &= v2Weight >= 0;

			if (overlaps) {
				v0Weight /= triangleArea;
				v1Weight /= triangleArea;
				v2Weight /= triangleArea;
				float r{points[0].c.r * v0Weight};
				r += points[1].c.r * v1Weight;
				r += points[2].c.r * v2Weight;

				float g{points[0].c.g * v0Weight};
				g += points[1].c.g * v1Weight;
				g += points[2].c.g * v2Weight;

				float b{points[0].c.b * v0Weight};
				b += points[1].c.b * v1Weight;
				b += points[2].c.b * v2Weight;

				uint32_t color{((uint32_t)(r * 255) << 16) + ((uint32_t)(g * 255) << 8) + (uint32_t)(b * 255)};
				backbuffer.basePixel[yPixel * backbuffer.width + xPixel] = color;
			}
		}
	}
}

void rasterizeTriangleImage(Backbuffer backbuffer, Backbuffer image, Depthbuffer dBuffer, Vertex points[3]) {
	int scale{1};
	Vec2 pointsVector[3] = {{points[0].p.x, points[0].p.y}, {points[1].p.x, points[1].p.y}, {points[2].p.x, points[2].p.y}};

	for (int yPixel{}; yPixel < backbuffer.height; yPixel++) {
		for (int xPixel{}; xPixel < backbuffer.width; xPixel++) {
			int xRenderPixel{xPixel / scale};
			int yRenderPixel{yPixel / scale};

			bool pointInside{true};
			float triangleArea{edgeFunction(pointsVector[0], pointsVector[1], pointsVector[2])};

			float v0Weight{};
			float v1Weight{};
			float v2Weight{};

			v0Weight = edgeFunction(pointsVector[1], pointsVector[2], {(float)xRenderPixel + 0.5f, (float)yRenderPixel + 0.5f});
			v1Weight = edgeFunction(pointsVector[2], pointsVector[0], {(float)xRenderPixel + 0.5f, (float)yRenderPixel + 0.5f});
			v2Weight = edgeFunction(pointsVector[0], pointsVector[1], {(float)(xRenderPixel + 0.5f), (float)yRenderPixel + 0.5f});

			Vec2 edge0{pointsVector[1] - pointsVector[0]};
			Vec2 edge1{pointsVector[2] - pointsVector[1]};
			Vec2 edge2{pointsVector[0] - pointsVector[2]};

			bool overlaps{true};

			overlaps &= v0Weight >= 0;
			overlaps &= v1Weight >= 0;
			overlaps &= v2Weight >= 0;

			if (overlaps) {
				v0Weight /= triangleArea;
				v1Weight /= triangleArea;
				v2Weight /= triangleArea;

				float* pxDepth{&dBuffer.basePixel[yPixel * backbuffer.width + xPixel]};
				float txDepth{points[0].p.z * v0Weight};
				txDepth += points[1].p.z * v1Weight;
				txDepth += points[2].p.z * v2Weight;

				if (txDepth > *pxDepth || txDepth < 0) {
					return;
				}
				*pxDepth = txDepth;

				float u{points[0].uv.x * v0Weight};
				u += points[1].uv.x * v1Weight;
				u += points[2].uv.x * v2Weight;

				float v{points[0].uv.y * v0Weight};
				v += points[1].uv.y * v1Weight;
				v += points[2].uv.y *  v2Weight;

				u *= image.width - 1;
				v *= image.height - 1;

				uint32_t xIndex{(uint32_t)round(u)};
				uint32_t yIndex{(uint32_t)round(v)};

				uint32_t color{image.basePixel[yIndex * image.width + xIndex]};

				uint32_t* pixel{&backbuffer.basePixel[yPixel * backbuffer.width + xPixel]};
				 *pixel = ((((color >> 16) & 0xFF) << 0) |
						   (((color >> 8) & 0xFF) << 8) |
						   (((color >> 0) & 0xFF) << 16) |
						   (((color >> 24) & 0xFF) << 24)
					   );
			}
		}
	}
}

void inputAssembler() {

}

Backbuffer* stretchBackbuffer(Backbuffer* backbuffer, uint32_t stretchW, uint32_t stretchH) {
	Backbuffer* resultBackbuffer = new Backbuffer;
	for (int yPixel{0}; yPixel < backbuffer->height * stretchH; yPixel++) {
		for (int xPixel{0}; xPixel < backbuffer->height * stretchW; xPixel++) {

		}
	}
	return resultBackbuffer;
}
