#ifndef PIPELINE_H
#define PIPELINE_H
/* Software rasterization pipeline. */

#include <stdlib.h>
#include "texturebuffer.h"

typedef struct {
    int x, y, w, h;
} SR_ViewPort;

typedef struct {
    TextureBuffer colorbuffer;
    double* depthbuffer;
} SR_FrameBuffer;

enum SR_PRIMITIVE_TYPE{
    SR_POINTS = 0, SR_LINES, SR_TRIANGLES
};
enum SR_DATA_TYPE{
    SR_BYTE = 0, SR_INT, SR_UINT, SR_FLOAT
};
enum SR_BUFFER_TYPE {
    SR_VERTEX_BUFFER = 0
};

void SR_Init();

void SR_SetViewPort(int x, int y, int w, int h);

void SR_GenBuffers(int *handles, size_t count);
void SR_SetBufferData(int handle, void* data, size_t arrsize);
void SR_BindBuffer(int buffertype, int handle);



#endif // PIPELINE_H
