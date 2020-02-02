#include "sre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static functions */

static inline void mixAttribsTriangle(SR_Vecf *attribs, double *lmbd, SR_Pipeline *pipeline)
{
    const size_t ATTRIB_COUNT = pipeline->vertexStageOutputCount;
    for (size_t i = 0; i < ATTRIB_COUNT; i++) {
	const SR_Vec4f aiV0 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*0) + i].vec4f;
	const SR_Vec4f aiV1 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*1) + i].vec4f;
	const SR_Vec4f aiV2 = pipeline->pVertexStageOutput[(ATTRIB_COUNT*2) + i].vec4f;

	attribs[i] = (SR_Vecf)(SR_Vec4f){
	    .x = lmbd[0]*aiV0.x + lmbd[1]*aiV1.x + lmbd[2]*aiV2.x,
	    .y = lmbd[0]*aiV0.y + lmbd[1]*aiV1.y + lmbd[2]*aiV2.y,
	    .z = lmbd[0]*aiV0.z + lmbd[1]*aiV1.z + lmbd[2]*aiV2.z,
	    .w = lmbd[0]*aiV0.w + lmbd[1]*aiV1.w + lmbd[2]*aiV2.w,
	};
    }
}

/* ~/Static functions/~ */


void SR_WritePixel(SR_FrameBuffer *buffer, const int *pos, void* value)
/* Writes the desired color values in the (x, y) coordinates of the color buffer. */
{   
    const int offset = (buffer->color.width * pos[1] + pos[0]);
    //printf("%d, %d\n", pos[2], buffer->depth.values[offset]);
    if (pos[2] <  buffer->depth.values[offset]) {
	buffer->depth.values[offset] = pos[2];
	SR_TexBufferWrite(&buffer->color, value, pos[0], pos[1]);
    }
}

void SR_WriteLine(SR_FrameBuffer *buffer, const int *pos, SR_Pipeline* pipeline)
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

void SR_WriteTriangle(SR_FrameBuffer *buffer, const int *pos, SR_Pipeline* pipeline)
/* Triangle rastierization using the pineda algorithm. */
{
    const int v0[] = {pos[0], pos[1], pos[2]};
    const int v1[] = {pos[3], pos[4], pos[5]};
    const int v2[] = {pos[6], pos[7], pos[8]};
    
    // Bounding box
    const int WIDTH = (int)buffer->color.width-1;
    const int HEIGHT = (int)buffer->color.height-1;
    int bx = v0[0]<v1[0] ? (v0[0]<v2[0] ? v0[0] : v2[0]) : (v1[0]<v2[0] ? v1[0] : v2[0]);
    int by = v0[1]<v1[1] ? (v0[1]<v2[1] ? v0[1] : v2[1]) : (v1[1]<v2[1] ? v1[1] : v2[1]);
    int bw = v0[0]>v1[0] ? (v0[0]>v2[0] ? v0[0] : v2[0]) : (v1[0]>v2[0] ? v1[0] : v2[0]);
    int bh = v0[1]>v1[1] ? (v0[1]>v2[1] ? v0[1] : v2[1]) : (v1[1]>v2[1] ? v1[1] : v2[1]);
    bx = (bx < WIDTH)  ? ((bx < 0) ? 0 : bx) : WIDTH;
    by = (by < HEIGHT) ? ((by < 0) ? 0 : by) : HEIGHT;
    bw = (bw < WIDTH)  ? ((bw < 0) ? 0 : bw) : WIDTH;
    bh = (bh < HEIGHT) ? ((bh < 0) ? 0 : bh) : HEIGHT;

    const int d02[2] = {v2[0] -v0[0], v2[1] -v0[1]};
    const int d01[2] = {v1[0] -v0[0], v1[1] -v0[1]};
    const int d12[2] = {v2[0] -v1[0], v2[1] -v1[1]};
    const int d20[2] = {v0[0] -v2[0], v0[1] -v2[1]};
    const double area = d02[0]*d01[1] - d02[1]*d01[0]; // Triangle Area x 2

    // Interpolated Vertex Attributes
    const size_t ATTRIB_COUNT = pipeline->vertexStageOutputCount;
    SR_Vecf attribs[ATTRIB_COUNT]; 
    
    int lpos[3];
    double lmbd[3]; // Barycentric coordinates
    int e01, e12, e20; // Edge values

    // Iterate bounding box
    for (lpos[1] = by; lpos[1] <= bh; lpos[1]++) {
	e12 = (bx-v1[0])*d12[1] - (lpos[1]-v1[1])*d12[0]; //vertex 0
	e20 = (bx-v2[0])*d20[1] - (lpos[1]-v2[1])*d20[0]; //vertex 1
	e01 = (bx-v0[0])*d01[1] - (lpos[1]-v0[1])*d01[0]; //vertex 2
	
	for (lpos[0] = bx; lpos[0] <= bw; lpos[0]++) {
	    if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0)) {
		// Calculate Barycentric coordinates
		lmbd[0] = (double)e12/area;
		lmbd[1] = (double)e20/area;
		lmbd[2] = (double)e01/area;
		
		lpos[2] = lmbd[0]*v0[2]+lmbd[1]*v1[2]+lmbd[2]*v2[2];

		// Attribute interpolation
		mixAttribsTriangle(attribs, lmbd, pipeline);
		
		// Fragment shading
		SR_Vec4f color = (SR_Vec4f){0.0, 0.0, 0.0, 0.0};
		(*(pipeline->fragmentShader))(ATTRIB_COUNT, attribs, &color);
		uint8_t texel[] = {color.x*255,
				   color.y*255,
				   color.z*255,
				   color.w*255};
		
		SR_WritePixel(buffer, lpos, &texel);
	    }
	    	    
	    e01 += d01[1];
	    e12 += d12[1];
	    e20 += d20[1];
	}
    }
}
