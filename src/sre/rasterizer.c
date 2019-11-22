#include <stdlib.h>
#include <string.h>
#include "texturebuffer.h"

void SR_WritePixel(SR_TextureBuffer *buffer, const size_t *pos, const SR_Texel *value)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{
    const size_t offset = (buffer->width * pos[1] + pos[0]) * buffer->fsize;
    memcpy(&buffer->values[offset], value, sizeof(&value));
}

void SR_WriteLine(SR_TextureBuffer *buffer, const size_t *pos, const SR_Texel *value)
/* Bresenheim Midpoint Line Rasterization. */
{
    int dx = pos[2] - pos[0];
    int dy = pos[3] - pos[1];
    int inc[2] = {1, 1};
    if (dx < 0) {
	dx = -dx;
	inc[0] = -1;
    }
    if (dy < 0) {
	dy = -dy;
	inc[1] = -1;
    }    

    // 0 <= f' <= 1
    int d = 2*dy - dx;
    int smallerIncr = 2*dy;        // Step into E | N
    int greaterIncr = 2*(dy - dx); // Step into NE
    int grIncX = inc[0];
    int smIncY = 0;
    // 1 < f'
    if (dy > dx) {
	d = dy - 2*dx;
	smallerIncr = greaterIncr;
	greaterIncr = -2*dx;
	grIncX = 0;
	smIncY = inc[1];
    }

    // Incremental Rasterization
    size_t linep[2] = {pos[0], pos[1]};
    while (linep[0] != pos[2] || linep[1] != pos[3]) {
	SR_WritePixel(buffer, (size_t*)&linep, value);
	if (d <= 0) {
	    d += smallerIncr;
	    linep[0] += inc[0];
	    linep[1] += smIncY;
	}
	else {       
	    d += greaterIncr;
	    linep[0] += grIncX;
	    linep[1] += inc[1];
	}
    }
}

void SR_WriteTriangle(SR_TextureBuffer *buffer, const size_t *pos, const SR_Texel *value)
/* Triangle rastierization using the pineda algorithm. */
{
    const size_t bx = pos[0]<pos[2] ? (pos[0]<pos[4] ? pos[0] : pos[4]) : (pos[2]<pos[4] ? pos[2] : pos[4]);
    const size_t by = pos[1]<pos[3] ? (pos[1]<pos[5] ? pos[1] : pos[5]) : (pos[3]<pos[5] ? pos[3] : pos[5]);
    const size_t bw = pos[0]>pos[2] ? (pos[0]>pos[4] ? pos[0] : pos[4]) : (pos[2]>pos[4] ? pos[2] : pos[4]);
    const size_t bh = pos[1]>pos[3] ? (pos[1]>pos[5] ? pos[1] : pos[5]) : (pos[3]>pos[5] ? pos[3] : pos[5]);
    
    const int d01[2] = {(int)pos[2] -(int)pos[0], (int)pos[3] -(int)pos[1]};
    const int d12[2] = {(int)pos[4] -(int)pos[2], (int)pos[5] -(int)pos[3]};
    const int d20[2] = {(int)pos[0] -(int)pos[4], (int)pos[1] -(int)pos[5]};

    size_t lpos[2];
    int e01, e12, e20;
    for (lpos[1] = by; lpos[1] <= bh; lpos[1]++) {
	e01 = (bx-(int)pos[0])*d01[1] - (lpos[1]-(int)pos[1])*d01[0];
	e12 = (bx-(int)pos[2])*d12[1] - (lpos[1]-(int)pos[3])*d12[0];
	e20 = (bx-(int)pos[4])*d20[1] - (lpos[1]-(int)pos[5])*d20[0];
	
	for (lpos[0] = bx; lpos[0] <= bw; lpos[0]++) {
	    if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0))
		SR_WritePixel(buffer, (size_t*)&lpos, value);
	    	    
	    e01 += d01[1];
	    e12 += d12[1];
	    e20 += d20[1];
	}
    }
}

void SR_WriteTriangleLine(SR_TextureBuffer *buffer, const size_t *pos, const SR_Texel *value)
/* Rasterize a triangle as a wireframe. */ 
{
    const size_t l01[4] = {pos[0], pos[1], pos[2], pos[3]};
    const size_t l12[4] = {pos[2], pos[3], pos[4], pos[5]};
    const size_t l20[4] = {pos[4], pos[5], pos[0], pos[1]};
    SR_WriteLine(buffer, (size_t*)&l01, value);
    SR_WriteLine(buffer, (size_t*)&l12, value);
    SR_WriteLine(buffer, (size_t*)&l20, value);
}
