#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Rasterizing primitives onto a texturebuffer. */

#include "texturebuffer.h"

void WritePixel(TextureBuffer *buffer, Texel value,
		unsigned int x, unsigned int y);

void WriteLine(TextureBuffer *buffer, Texel value,
	       unsigned int x0, unsigned int y0,
	       unsigned int x1, unsigned int y1);

#endif // RASTERIZER_H
