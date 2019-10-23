#include "rasterizer.h"
#include <stdlib.h>

void WritePixel(ColorBuffer *buffer,
		uint_fast32_t x, uint_fast32_t y,
		uint8_t b, uint8_t g, uint8_t r, uint8_t a)
/* Writes the desired color values in the x,y coordinates of the color buffer. */
{
    const size_t offset = (buffer->width * y + x) * 4;
    buffer->array[offset + 0] = b;
    buffer->array[offset + 1] = g;
    buffer->array[offset + 2] = r;
    buffer->array[offset + 3] = a;
}

void WriteLine(ColorBuffer *buffer,
	       uint_fast32_t x0, uint_fast32_t y0,
	       uint_fast32_t x1, uint_fast32_t y1, uint8_t value)
/* Bresenheim Midpoint Line Rasterization.  */
{
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    uint32_t x = x0;
    uint32_t y = y0;
    int32_t xi = 1;
    int32_t yi = 1;
    if (dx < 0) {
	dx = -dx;
	xi = -1;
    }
    if (dy < 0) {
	dy = -dy;
	yi = -1;
    }    

    // 0 <= f' <= 1
    int32_t d = 2*dy - dx;
    int32_t smallerIncr = 2*dy;        // Step into E | N
    int32_t greaterIncr = 2*(dy - dx); // Step into NE
    int32_t grIncX = xi;
    int32_t smIncY = 0;
    // 1 < f'
    if (dy > dx) {
	d = dy - 2*dx;
	smallerIncr = greaterIncr;
	greaterIncr = -2*dx;
	grIncX = 0;
	smIncY = yi;
    }

    // Incremental Rasterization
    while (x != x1 || y != y1) {
	WritePixel(buffer, x, y, value, value, value, 255);
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
