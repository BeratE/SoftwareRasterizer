#ifndef COMMON_H
#define COMMON_H
/* Common shared definitions, typedefs and structs. */

#include "stdlib.h"

enum SR_BUFFER_TYPE { SR_VERTEX_BUFFER,
		      SR_INDEX_BUFFER};

enum SR_PRIMITIVE_TYPE {SR_POINTS = 1,
			SR_LINES = 2,
			SR_TRIANGLES = 3};

enum SR_RENDER_TARGET_BIT {SR_COLOR_BUFFER_BIT = 1,
			   SR_DEPTH_BUFFER_BIT = 2};

typedef struct {
    size_t count;  // Package size
    size_t stride; // Stride in bytes
    size_t offset; // Offset in bytes
} SR_VertAttrib;

typedef struct {
    double* vertexBuffer;
    size_t* indexBuffer;
    size_t vertexBufferSize, indexBufferSize;
    SR_VertAttrib* vertAttribs;
    size_t vertAttribCount;
} SR_VAO;

#endif // COMMON_H
