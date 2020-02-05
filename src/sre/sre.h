#ifndef SRE_H
#define SRE_H
/* Software rasterization pipeline. */

#include "types.h"

// Setup, Shutdown
void SR_Init();
void SR_Shutdown();

// Framebuffers
SR_FrameBuffer SR_GetFrameBuffer();
void SR_SetViewPort(int w, int h);
void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask);

// Vertex Array Objects
size_t SR_GenVertexArray();
void SR_BindVertexArray(size_t handle);
void SR_DestroyVertexArray(size_t handle);
void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size);

// Vertex Attributes
void SR_SetVertexAttributeCount(size_t count);
void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset);

// Pipeline
void SR_BindShader(enum SR_SHADER_TYPE shader_type, SR_ShaderCB shader);
void SR_SetVertexStageOutputCount(size_t count);
void SR_SetVertexStageOutput(size_t index, SR_Vecf* value);
void SR_DrawArray(enum SR_PRIMITIVE_TYPE type, size_t count, size_t startxindex);

// Texture buffers
SR_TexBuffer2D SR_TexBufferCreate(size_t width, size_t height, uint16_t format);
SR_TexBuffer2D SR_TexBufferCopy(const SR_TexBuffer2D* buffer);
void SR_TexBufferRead(SR_TexBuffer2D *buffer, void* outValue, size_t x, size_t y);
void SR_TexBufferSample(SR_TexBuffer2D *buffer, void* outValue, float x, float y);
void SR_TexBufferWrite(SR_TexBuffer2D *buffer, const void *value, size_t x, size_t y);
void SR_TexBufferClear(SR_TexBuffer2D *buffer, const void *value);
void SR_TexBufferFree(SR_TexBuffer2D *buffer);
size_t SR_TexBufferFormatSize(const SR_TexBuffer2D *buffer);
size_t SR_TexBufferSize(const SR_TexBuffer2D *buffer);

// Rasterization
void SR_WritePixel(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const void* value);
void SR_WriteLine(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const SR_Pipeline* pipeline);
void SR_WriteTriangle(SR_FrameBuffer *buffer, const SR_VecScreen *pos, const SR_Pipeline* pipeline);

#endif // SRE_H
