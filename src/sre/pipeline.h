#ifndef PIPELINE_H
#define PIPELINE_H
/* Software rasterization pipeline. */

#include "texturebuffer.h"

typedef struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} SR_FrameBuffer;

void SR_Init();
void SR_Shutdown();

void SR_SetViewPort(int w, int h);
SR_FrameBuffer SR_FetchFrameBuffer();

void SR_DrawCube();


#endif // PIPELINE_H
