#include "rasterizer.h"
#include <stdlib.h>
#include <string.h>

void WritePixel(TextureBuffer *buffer, Texel value,
		unsigned int x, unsigned int y)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{
    const size_t offset = (buffer->width * y + x) * buffer->fsize;
    memcpy(&buffer->values[offset], &value, sizeof(value));
}

void WriteLine(TextureBuffer *buffer, Texel value,
	       unsigned int x0, unsigned int y0,
	       unsigned int x1, unsigned int y1)
/* Bresenheim Midpoint Line Rasterization.  */
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    int yi = 1;
    if (dx < 0) {
	dx = -dx;
	xi = -1;
    }
    if (dy < 0) {
	dy = -dy;
	yi = -1;
    }    

    // 0 <= f' <= 1
    int d = 2*dy - dx;
    int smallerIncr = 2*dy;        // Step into E | N
    int greaterIncr = 2*(dy - dx); // Step into NE
    int grIncX = xi;
    int smIncY = 0;
    // 1 < f'
    if (dy > dx) {
	d = dy - 2*dx;
	smallerIncr = greaterIncr;
	greaterIncr = -2*dx;
	grIncX = 0;
	smIncY = yi;
    }

    unsigned int x = x0;
    unsigned int y = y0;

    // Incremental Rasterization
    while (x != x1 || y != y1) {
	WritePixel(buffer, value, x, y);
	if (d <= 0) {
	    d += smallerIncr;
	    x += xi;
	    y += smIncY;
	}
	else {       
	    d += greaterIncr;
	    x += grIncX;
	    y += yi;
	}
    }
}
