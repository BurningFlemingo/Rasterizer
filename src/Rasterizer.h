#pragma once

#include <stdint.h>
#include <math.h>

#include "Math.h"

struct Backbuffer {
	uint32_t* basePixel;
	uint32_t width;
	uint32_t height;
};
struct Depthbuffer {
	float* basePixel;
	uint32_t width;
	uint32_t height;
};

struct Point3D {
	float x;
	float y;
	float z;
};

struct Color4D {
	float r;
	float g;
	float b;
	float a;
};

struct UV {
	float x;
	float y;
};

struct Vertex {
	Point3D p;
	Color4D c;
	UV uv;
};

void clearBuffer(Backbuffer backbuffer, float r, float g, float b);
void clearBuffer(Depthbuffer dBuffer);

void rasterizeTriangle(Backbuffer backbuffer, Vertex points[3]);
void rasterizeTriangleImage(Backbuffer backbuffer, Backbuffer image, Depthbuffer dBuffer, Vertex points[3]);

// creates new backbuffer
Backbuffer* stretchBackbuffer(Backbuffer* backbuffer, uint32_t stretchW, uint32_t stretchH);
