#include <stdlib.h>
#include <string.h>
#include "texturebuffer.h"

void SR_WritePixel(SR_TextureBuffer *buffer, const int *pos, const SR_Texel *value)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{
    const int offset = (buffer->width * pos[1] + pos[0]) * buffer->fsize;
    memcpy(&buffer->values[offset], value, sizeof(&value));
}

void SR_WriteLine(SR_TextureBuffer *buffer, const int *pos, const SR_Texel *value)
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
    int linep[2] = {pos[0], pos[1]};
    while (linep[0] != pos[2] || linep[1] != pos[3]) {
	SR_WritePixel(buffer, linep, value);
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

void SR_WriteTriangle(SR_TextureBuffer *buffer, const int *pos, const SR_Texel *value)
/* Triangle rastierization using the pineda algorithm. */
{
    SR_Texel random = (SR_Texel)((SR_RGBA8){.r= rand()%255, .g = rand()%255, .b = rand()%255, .a= 255});
    
    // Bounding box
    const int WIDTH = (int)buffer->width-1;
    const int HEIGHT = (int)buffer->height-1;
    int bx = pos[0]<pos[2] ? (pos[0]<pos[4] ? pos[0] : pos[4]) : (pos[2]<pos[4] ? pos[2] : pos[4]);
    int by = pos[1]<pos[3] ? (pos[1]<pos[5] ? pos[1] : pos[5]) : (pos[3]<pos[5] ? pos[3] : pos[5]);
    int bw = pos[0]>pos[2] ? (pos[0]>pos[4] ? pos[0] : pos[4]) : (pos[2]>pos[4] ? pos[2] : pos[4]);
    int bh = pos[1]>pos[3] ? (pos[1]>pos[5] ? pos[1] : pos[5]) : (pos[3]>pos[5] ? pos[3] : pos[5]);
    bx = (bx < WIDTH)  ? ((bx < 0) ? 0 : bx) : WIDTH;
    by = (by < HEIGHT) ? ((by < 0) ? 0 : by) : HEIGHT;
    bw = (bw < WIDTH)  ? ((bw < 0) ? 0 : bw) : WIDTH;
    bh = (bh < HEIGHT) ? ((bh < 0) ? 0 : bh) : HEIGHT;
    
    const int d01[2] = {(int)pos[2] -(int)pos[0], (int)pos[3] -(int)pos[1]};
    const int d12[2] = {(int)pos[4] -(int)pos[2], (int)pos[5] -(int)pos[3]};
    const int d20[2] = {(int)pos[0] -(int)pos[4], (int)pos[1] -(int)pos[5]};

    int lpos[2];
    int e01, e12, e20;
    for (lpos[1] = by; lpos[1] <= bh; lpos[1]++) {
	e01 = (bx-pos[0])*d01[1] - (lpos[1]-pos[1])*d01[0];
	e12 = (bx-pos[2])*d12[1] - (lpos[1]-pos[3])*d12[0];
	e20 = (bx-pos[4])*d20[1] - (lpos[1]-pos[5])*d20[0];
	
	for (lpos[0] = bx; lpos[0] <= bw; lpos[0]++) {
	    if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0))
		SR_WritePixel(buffer, lpos, &random);
	    	    
	    e01 += d01[1];
	    e12 += d12[1];
	    e20 += d20[1];
	}
    }
}

void SR_WriteTriangleLine(SR_TextureBuffer *buffer, const size_t *pos, const SR_Texel *value)
/* Rasterize a triangle as a wireframe. */ 
{
    const int l01[4] = {pos[0], pos[1], pos[2], pos[3]};
    const int l12[4] = {pos[2], pos[3], pos[4], pos[5]};
    const int l20[4] = {pos[4], pos[5], pos[0], pos[1]};
    SR_WriteLine(buffer, l01, value);
    SR_WriteLine(buffer, l12, value);
    SR_WriteLine(buffer, l20, value);
}
