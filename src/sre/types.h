#ifndef TYPES_H
#define TYPES_H
/* Common shared definitions, typedefs and structs. */

#include <stdint.h>
#include <stdlib.h>

/* Macros */

#define MIN3(a, b, c) (a < b) ? (a < c ? a : c) : (b < c ? b : c)
#define MAX3(a, b, c) (a > b) ? (a > c ? a : c) : (b > c ? b : c)
#define CLAMP(x, a, b) (x < b) ? ((x < a) ? a : x) : b // clamp x between (a, b)

/* ~/Macros/~ */

// Common Constants and Enumerations
enum SR_PRIMITIVE_TYPE{
    SR_POINTS    = 1,
    SR_LINES     = 2,
    SR_TRIANGLES = 3};

enum SR_BUFFER_TYPE{
    SR_VERTEX_BUFFER,
    SR_INDEX_BUFFER};

enum SR_SHADER_TYPE{
    SR_VERTEX_SHADER,
    SR_FRAGMENT_SHADER };

enum SR_RENDER_TARGET_BIT{
    SR_COLOR_BUFFER_BIT = 1,
    SR_DEPTH_BUFFER_BIT = 2};


// Texturebuffer
typedef struct {
    size_t width;   
    size_t height;
    uint16_t format;
    uint8_t *values;
} SR_TexBuffer2D;
#define SR_NULL_TEXBUFFER (SR_TexBuffer2D){.width=0,.height=0,.format=0,.values=NULL}

// Texture Format
enum SR_TEXTURE_FORMAT_MASK {
    SR_TEX_FMTM_TYPE   = 0xF000,
    SR_TEX_FMTM_NCOMPS = 0x00F0,
    SR_TEX_FMTM_NBYTES = 0x000F,
};
enum SR_TEXTURE_FORMAT_TYPE {
    SR_TEX_TYPE_UINT  = 0x0000,
    SR_TEX_TYPE_FLOAT = 0x1000,
};
enum SR_TEXTURE_FORMAT {
    // [Type | Num Components | 0 | Num Bytes]
    // Composite Integer Types - 0
    SR_TEX_FORMAT_R8     = SR_TEX_TYPE_UINT |(1 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RG8    = SR_TEX_TYPE_UINT |(2 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RGB8   = SR_TEX_TYPE_UINT |(3 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RGBA8  = SR_TEX_TYPE_UINT |(4 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_R16    = SR_TEX_TYPE_UINT |(1 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RG16   = SR_TEX_TYPE_UINT |(2 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RGB16  = SR_TEX_TYPE_UINT |(3 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RGBA16 = SR_TEX_TYPE_UINT |(4 << 4) | sizeof(uint16_t),
    // Floating Point Types - 1
    SR_TEX_FORMAT_F32    = SR_TEX_TYPE_FLOAT | (1 << 4) | sizeof(float),
};

// Common vector types
typedef struct { double x; }          SR_Vec1f;
typedef struct { double x, y; }       SR_Vec2f;
typedef struct { double x, y, z; }    SR_Vec3f;
typedef struct { double x, y, z, w; } SR_Vec4f;
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
    double* vertexBuffer;
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
typedef void(*SR_ShaderCB)(size_t attributeCount,
			 SR_Vecf* attributes,
			 SR_Vec4f* output);

// Collection of shader objects in a pipeline
typedef struct {
    SR_ShaderCB vertexShader; // Currently bound vertex shader callback
    SR_ShaderCB fragmentShader; // Currently bound fragment shader callback
    size_t vertexStageOutputCount; // Number of vertex output elements
    SR_Vecf *currVertexStageOutput; // Output of single vertex shader
    SR_Vecf *pVertexStageOutput; // Pointer to collected vertex output
} SR_Pipeline;

// Write functions
typedef void(*SR_Write)(SR_FrameBuffer *buffer,
			const SR_VecScreen *pos,
			const SR_Pipeline *pipeline);

#endif // TYPES_H
