#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Methods for rasterizing primitives onto a colorbuffer. */

#include "colorbuffer.h"

void WritePixel(ColorBuffer *buffer,
		unsigned int x, unsigned int y,
		unsigned char b, unsigned char g, unsigned char r, unsigned char a);

void WriteLine(ColorBuffer *buffer,
	       unsigned int x0, unsigned int y0,
	       unsigned int x1, unsigned int y1, unsigned char value);

#endif // RASTERIZER_H
