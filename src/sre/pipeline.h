#ifndef PIPELINE_H
#define PIPELINE_H
/* Software rasterization pipeline. */

#include "texturebuffer.h"

typedef struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} SR_Framebuffer;

void SR_Init();
void SR_Shutdown();

void SR_SetViewPort(int w, int h);

void SR_DrawCube();


#endif // PIPELINE_H
