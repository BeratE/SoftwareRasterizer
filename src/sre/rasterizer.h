#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Rasterizing primitives onto a texturebuffer. */

#include "texturebuffer.h"

void SR_WritePixel(TextureBuffer *buffer, Texel value,
		unsigned int x, unsigned int y);

void SR_WriteLine(TextureBuffer *buffer, Texel value,
	       unsigned int x0, unsigned int y0,
	       unsigned int x1, unsigned int y1);

void SR_WriteTriangle(TextureBuffer *buffer, Texel value,
		   unsigned int x0, unsigned int y0,
		   unsigned int x1, unsigned int y1,
		   unsigned int x2, unsigned int y2);

#endif // RASTERIZER_H
