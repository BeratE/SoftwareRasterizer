#include "sre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static functions */

static inline void mixAttribsTriangle(SR_Vecf *attribs, const SR_VecScreen *fpos, const SR_VecScreen *pos,
				      const float *baryc, const SR_Pipeline *pipeline)
{
    const size_t ATTRIB_COUNT = pipeline->vertexStageOutputCount;
    for (size_t i = 0; i < ATTRIB_COUNT; i++) {
	const SR_Vec4f aiV0 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*0) + i].vec4f;
	const SR_Vec4f aiV1 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*1) + i].vec4f;
	const SR_Vec4f aiV2 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*2) + i].vec4f;

	attribs[i] = (SR_Vecf)(SR_Vec4f){
	    .x = fpos->z*(baryc[0]*(aiV0.x/pos[0].z) + baryc[1]*(aiV1.x/pos[1].z) + baryc[2]*(aiV2.x/pos[2].z)),
	    .y = fpos->z*(baryc[0]*(aiV0.y/pos[0].z) + baryc[1]*(aiV1.y/pos[1].z) + baryc[2]*(aiV2.y/pos[2].z)),
	    .z = fpos->z*(baryc[0]*(aiV0.z/pos[0].z) + baryc[1]*(aiV1.z/pos[1].z) + baryc[2]*(aiV2.z/pos[2].z)),
	    .w = fpos->z*(baryc[0]*(aiV0.w/pos[0].z) + baryc[1]*(aiV1.w/pos[1].z) + baryc[2]*(aiV2.w/pos[2].z)),
	};
    }
}

/* ~/Static functions/~ */


void SR_WritePixel(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const void* value)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{   
    float depth;
    SR_TexBufferRead(&buffer->depth, &depth, pos->x, pos->y);
    if (pos->z < depth) {
	SR_TexBufferWrite(&buffer->depth, &pos->z, pos->x, pos->y);
	SR_TexBufferWrite(&buffer->color, value, pos->x, pos->y);
    }
}

void SR_WriteLine(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const SR_Pipeline* pipeline)
/* Bresenheim Midpoint Line Rasterization. */
{
/*
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
	SR_WritePixel(buffer, linep, shader);
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
*/
}

void SR_WriteTriangle(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const SR_Pipeline* pipeline)
/* Triangle rastierization using the pineda algorithm. */
{
    const int WIDTH = (int)buffer->color.width-1;
    const int HEIGHT = (int)buffer->color.height-1;
    // Bounding box
    int bx = MIN3(pos[0].x, pos[1].x, pos[2].x);
    int by = MIN3(pos[0].y, pos[1].y, pos[2].y);
    int bw = MAX3(pos[0].x, pos[1].x, pos[2].x);
    int bh = MAX3(pos[0].y, pos[1].y, pos[2].y);
    bx = CLAMP(bx, 0, WIDTH);
    by = CLAMP(by, 0, HEIGHT);
    bw = CLAMP(bw, 0, WIDTH);
    bh = CLAMP(bh, 0, HEIGHT);

    // Edges
    const int d02[2] = {pos[2].x -pos[0].x, pos[2].y -pos[0].y};
    const int d01[2] = {pos[1].x -pos[0].x, pos[1].y -pos[0].y};
    const int d12[2] = {pos[2].x -pos[1].x, pos[2].y -pos[1].y};
    const int d20[2] = {pos[0].x -pos[2].x, pos[0].y -pos[2].y};
    const int area = d02[0]*d01[1] - d02[1]*d01[0]; // Triangle Area x 2

    // Interpolated Vertex Attributes
    const size_t ATTRIB_COUNT = pipeline->vertexStageOutputCount;
    SR_Vecf attribs[ATTRIB_COUNT]; 
    
    SR_VecScreen fpos; // Fragment position
    float baryc[3];    // Barycentric coordinates
    int e01, e12, e20; // Edge values

    // Iterate bounding box
    for (fpos.y = by; fpos.y <= bh; fpos.y++) {
	e12 = (bx-pos[1].x)*d12[1] - (fpos.y-pos[1].y)*d12[0]; //vertex 0
	e20 = (bx-pos[2].x)*d20[1] - (fpos.y-pos[2].y)*d20[0]; //vertex 1
	e01 = (bx-pos[0].x)*d01[1] - (fpos.y-pos[0].y)*d01[0]; //vertex 2
	
	for (fpos.x = bx; fpos.x <= bw; fpos.x++) {
	    if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0)) {
		// Calculate Barycentric coordinates
		baryc[0] = (float)e12/area;
		baryc[1] = (float)e20/area;
		baryc[2] = (float)e01/area;

		// Z-Interpolation
		fpos.z = 1.0 / (baryc[0]/pos[0].z+baryc[1]/pos[1].z+baryc[2]/pos[2].z);
		
		// Attribute interpolation
		mixAttribsTriangle(attribs, &fpos, pos, baryc, pipeline);
		
		// Fragment shading
		SR_Vec4f color = (SR_Vec4f){0.0, 0.0, 0.0, 0.0};
		(*(pipeline->fragmentShader))(ATTRIB_COUNT, attribs, &color);
		uint8_t texel[] = {color.x*255,
				   color.y*255,
				   color.z*255,
				   color.w*255};
		
		SR_WritePixel(buffer, &fpos, &texel);
	    }
	    	    
	    e01 += d01[1];
	    e12 += d12[1];
	    e20 += d20[1];
	}
    }
}
