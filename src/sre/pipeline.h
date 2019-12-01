#ifndef PIPELINE_H
#define PIPELINE_H
/* Software rasterization pipeline. */

#include "common.h"
#include "texturebuffer.h"

void SR_Init();
void SR_Shutdown();

void SR_SetViewPort(int w, int h);

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask);
void SR_Blit(enum SR_RENDER_TARGET_BIT bufferbit, SR_TextureBuffer *buffer);

size_t SR_GenVertexArray();
void SR_BindVertexArray(size_t handle);

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size);

void SR_SetVertexAttributeCount(size_t count);
void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset);

void SR_DrawArrays(enum SR_PRIMITIVE_TYPE type, size_t count);

#endif // PIPELINE_H
