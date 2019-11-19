#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Rasterizing primitives onto a texturebuffer. */

#include "texturebuffer.h"

void SR_WritePixel(SR_TextureBuffer *buffer,SR_Texel value,
		   unsigned int x, unsigned int y);

void SR_WriteLine(SR_TextureBuffer *buffer, SR_Texel value,
		  unsigned int x0, unsigned int y0,
		  unsigned int x1, unsigned int y1);

void SR_WriteTriangle(SR_TextureBuffer *buffer, SR_Texel value,
		      unsigned int x0, unsigned int y0,
		      unsigned int x1, unsigned int y1,
		      unsigned int x2, unsigned int y2);

void SR_WriteTriangleLine(SR_TextureBuffer *buffer, SR_Texel value,
			  unsigned int x0, unsigned int y0,
			  unsigned int x1, unsigned int y1,
			  unsigned int x2, unsigned int y2);

#endif // RASTERIZER_H
