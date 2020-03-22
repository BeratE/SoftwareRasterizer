/* Declarations and definitions of common typedefs and structs. */

#ifndef SRETYPES_H
#define SRETYPES_H

#include <stdint.h>
#include <stdlib.h>

// Texturebuffer
typedef struct {
    size_t width;   
    size_t height;
    uint16_t format;
    uint8_t *values;
} SR_TexBuffer2D;
#define SR_NULL_TEXBUFFER (SR_TexBuffer2D){.width=0,.height=0,.format=0,.values=NULL}

// Common vector types
typedef struct { float x; }          SR_Vec1f;
typedef struct { float x, y; }       SR_Vec2f;
typedef struct { float x, y, z; }    SR_Vec3f;
typedef struct { float x, y, z, w; } SR_Vec4f;
typedef union {
    SR_Vec1f vec1f;
    SR_Vec2f vec2f;
    SR_Vec3f vec3f;
    SR_Vec4f vec4f;
} SR_Vecf;

// Composite vector types
typedef struct {
    int x, y;
    float z;
} SR_VecScreen;

// Vertex attribute data
typedef struct {
    size_t count;  // Package size
    size_t stride; // Stride in bytes
    size_t offset; // Offset in bytes
} SR_VertexAttribute;

// Vertex, index and attribute data collection
typedef struct {
    float* vertexBuffer;
    size_t* indexBuffer;
    size_t vertexBufferSize;
    size_t indexBufferSize;
    size_t attributesCount;
    SR_VertexAttribute* attributes;
} SR_VertexArray; 

// Framebuffer 
typedef struct {
    SR_TexBuffer2D color;
    SR_TexBuffer2D depth;
} SR_FrameBuffer;

// Shader functions
typedef void(*SR_ShaderCB)(size_t attributeCount, // Number of input attributes
			   SR_Vecf* attributes, // Array of input attribes
			   SR_Vec4f* output); // Single vec4 output attribute

// Collection of shader objects in a pipeline
typedef struct {
    SR_ShaderCB vertexShader; // Currently bound vertex shader callback
    SR_ShaderCB fragmentShader; // Currently bound fragment shader callback
    size_t vertexStageOutputCount; // Number of vertex output elements
    SR_Vecf *currVertexStageOutput; // Output of single vertex shader
    SR_Vecf *pVertexStageOutput; // Pointer to collected vertex output
} SR_Pipeline;

// Write functions
typedef void (*SR_Write)(SR_FrameBuffer *buffer, const SR_VecScreen *pos,
                         const SR_Pipeline *pipeline);

// Model loading
typedef struct {
    int primitiveType;
    int *indices;
} SR_Face;

typedef struct {
    char *name;
    size_t nFaces;
    size_t nTextureUVs;
    size_t nVertices;
    size_t nNormals;
    SR_Face *faces;
    float *textureUVs;
    float *vertices;
    float *normals;
} SR_Mesh;

#endif // SRETYPES_H
