#ifndef COMMON_H
#define COMMON_H
/* Common shared definitions, typedefs and structs. */

#include "stdlib.h"
#include "texturebuffer.h"

enum SR_PRIMITIVE_TYPE {SR_POINTS = 1,
			SR_LINES,
			SR_TRIANGLES};

enum SR_BUFFER_TYPE { SR_VERTEX_BUFFER,
		      SR_INDEX_BUFFER};

enum SR_SHADER_TYPE { SR_VERTEX_SHADER,
		      SR_FRAGMENT_SHADER };

enum SR_RENDER_TARGET_BIT {SR_COLOR_BUFFER_BIT = 1,
			   SR_DEPTH_BUFFER_BIT = 2};

/* Common vector types. */ 
typedef struct { double x; }          SR_Vec1f;
typedef struct { double x, y; }       SR_Vec2f;
typedef struct { double x, y, z; }    SR_Vec3f;
typedef struct { double x, y, z, w; } SR_Vec4f;
typedef union {
    SR_Vec1f vec1f;
    SR_Vec2f vec2f;
    SR_Vec3f vec3f;
    SR_Vec4f vec4f;
} SR_VecUnion;

/* Shaders (size_t attributeCount, SR_Vector* attributes, SR_Vec4f* output) */
typedef void(*SR_Shader)(size_t, SR_VecUnion*, SR_Vec4f*);

/* Vertex Array Data*/
typedef struct {
    size_t count;  // Package size
    size_t stride; // Stride in bytes
    size_t offset; // Offset in bytes
} SR_VertexAttribute;

typedef struct {
    double* vertexBuffer;
    size_t* indexBuffer;
    size_t vertexBufferSize;
    size_t indexBufferSize;
    size_t attributesCount;
    SR_VertexAttribute* attributes;
} SR_VertexArray;

typedef struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} SR_FrameBuffer;

#endif // COMMON_H
