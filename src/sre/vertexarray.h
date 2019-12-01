#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
/* Vertex Buffer Data */

#include <stdlib.h>
#include "common.h"

void SR_InitVertexArray();
void SR_ShutdownVertexArray();

size_t SR_GenVertexArray();
void SR_BindVertexArray(size_t handle);

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size);

void SR_SetVertexAttributeCount(size_t count);
void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset);

#endif // VERTEX_ARRAY_H
