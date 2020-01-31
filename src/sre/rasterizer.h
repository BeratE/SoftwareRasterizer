#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Rasterizing primitives onto a framebuffer. 
 * Every function takes a pointer to an array of positions with 3 components each, 
 * depending on the type of primitve to draw.*/

#include "types.h"

typedef void(*SR_Write)(SR_FrameBuffer*, const int*, const SR_Shader shader);

void SR_WritePixel(SR_FrameBuffer *buffer, const int *pos, const SR_Texel *value);

void SR_WriteLine(SR_FrameBuffer *buffer, const int *pos, const SR_Shader shader);
void SR_WriteTriangle(SR_FrameBuffer *buffer, const int *pos, const SR_Shader shader);
void SR_WriteTriangleLine(SR_FrameBuffer *buffer, const int* pos,  const SR_Shader shader);

#endif // RASTERIZER_H
