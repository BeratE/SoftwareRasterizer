#include <stdlib.h>
#include <string.h>
#include "texturebuffer.h"

void SR_WritePixel(SR_TextureBuffer *buffer, SR_Texel value,
		   unsigned int x, unsigned int y)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{
    const size_t offset = (buffer->width * y + x) * buffer->fsize;
    memcpy(&buffer->values[offset], &value, sizeof(value));
}

void SR_WriteLine(SR_TextureBuffer *buffer, SR_Texel value,
		  unsigned int x0, unsigned int y0,
		  unsigned int x1, unsigned int y1)
/* Bresenheim Midpoint Line Rasterization. */
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
	SR_WritePixel(buffer, value, x, y);
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

void SR_WriteTriangle(SR_TextureBuffer *buffer,SR_Texel value,
		      unsigned int x0, unsigned int y0,
		      unsigned int x1, unsigned int y1,
		      unsigned int x2, unsigned int y2)
/* Triangle rastierization using the pineda algorithm. */
{
    // Bounding rectangle
    const int bx = (int)(x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2));
    const int by = (int)(y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2));
    const int bw = (int)(x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2));
    const int bh = (int)(y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2));
    
    const int dx01 = (int)x1 -(int)x0;
    const int dx12 = (int)x2 -(int)x1;
    const int dx20 = (int)x0 -(int)x2;
    const int dy01 = (int)y1 -(int)y0;
    const int dy12 = (int)y2 -(int)y1;
    const int dy20 = (int)y0 -(int)y2;

    int e01, e12, e20; // Edge functions
    for (int y = by; y <= bh; y++) {
	e01 = (bx-(int)x0)*dy01 - (y-(int)y0)*dx01;
	e12 = (bx-(int)x1)*dy12 - (y-(int)y1)*dx12;
	e20 = (bx-(int)x2)*dy20 - (y-(int)y2)*dx20;
	
	for (int x = bx; x <= bw; x++) {
	    if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0))
		SR_WritePixel(buffer, value, x, y);
	    	    
	    e01 += dy01;
	    e12 += dy12;
	    e20 += dy20;
	}
    }
}

void SR_WriteTriangleLine(SR_TextureBuffer *buffer, SR_Texel value,
			  unsigned int x0, unsigned int y0,
			  unsigned int x1, unsigned int y1,
			  unsigned int x2, unsigned int y2)
/* Rasterize a triangle as a wireframe. */ 
{
    SR_WriteLine(buffer, value, x0, y0, x1, y1);
    SR_WriteLine(buffer, value, x1, y1, x2, y2);
    SR_WriteLine(buffer, value, x2, y2, x0, y0);
}
