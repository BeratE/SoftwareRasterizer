#ifndef PIPELINE_H
#define PIPELINE_H
/* Software rasterization pipeline. */

#include "texturebuffer.h"

enum SR_PRIMITIVE_TYPE {SR_POINTS = 1,
			SR_LINES = 2,
			SR_TRIANGLES = 3};
enum SR_RENDER_TARGET_BIT {SR_COLOR_BUFFER_BIT = 1,
			   SR_DEPTH_BUFFER_BIT = 2};
typedef struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} SR_FrameBuffer;

void SR_Init();
void SR_Shutdown();

void SR_SetViewPort(int w, int h);

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask);
SR_TextureBuffer SR_Blit(enum SR_RENDER_TARGET_BIT bufferbit);

void SR_DrawArrays(enum SR_PRIMITIVE_TYPE type, size_t count);




#endif // PIPELINE_H
