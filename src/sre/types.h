#ifndef TYPES_H
#define TYPES_H
/* Common shared definitions, typedefs and structs. */

#include <stdint.h>
#include "stdlib.h"

/* Common Constants and Enumerations */
enum SR_PRIMITIVE_TYPE {SR_POINTS = 1,
			SR_LINES,
			SR_TRIANGLES};

enum SR_BUFFER_TYPE { SR_VERTEX_BUFFER,
		      SR_INDEX_BUFFER};

enum SR_SHADER_TYPE { SR_VERTEX_SHADER,
		      SR_FRAGMENT_SHADER };

enum SR_RENDER_TARGET_BIT {SR_COLOR_BUFFER_BIT = 1,
			   SR_DEPTH_BUFFER_BIT = 2};


/* Texturebuffer */
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char fsize;
    unsigned char *values;
} SR_TextureBuffer;
#define SR_NULL_TEXTUREBUFFER (SR_TextureBuffer){.width=0,.height=0,.fsize=0,.values=NULL}

/* Texture Format */
// Avaliable Texture formats defined as sizes (in bytes).
#define SR_TEXTURE_FORMAT_R8     1
#define SR_TEXTURE_FORMAT_RG16   2
#define SR_TEXTURE_FORMAT_RGB8   3
#define SR_TEXTURE_FORMAT_RGBA8  4
#define SR_TEXTURE_FORMAT_RGB16  6
#define SR_TEXTURE_FORMAT_RGBA16 8
/* Texture Data */
typedef struct {
    uint8_t r;
} SR_R8;
typedef struct {
    uint16_t r;
} SR_R16;
typedef struct {
    uint8_t r, g, b;
} SR_RGB8;
typedef struct {
    uint8_t r, g, b, a;
} SR_RGBA8;
typedef struct {
    uint16_t r, g, b;
} SR_RGB16;
typedef struct {
    uint16_t r, g, b, a;
} SR_RGBA16;
typedef union {
    SR_R8     r8;
    SR_R16    r16;
    SR_RGB8   rgb8;
    SR_RGBA8  rgba8;
    SR_RGB16  rgb16;
    SR_RGBA16 rgba16;
} SR_Texel;


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
} SR_Vecf;

/* Shader functions*/
typedef void(*SR_Shader)(size_t, SR_Vecf*, SR_Vec4f*);

/* Collection of shader objects in a pipeline. */
typedef struct {
    SR_Shader vertexShader;
    SR_Shader fragmentShader;
} SR_Pipeline;

/* Vertex attribute data. */
typedef struct {
    size_t count;  // Package size
    size_t stride; // Stride in bytes
    size_t offset; // Offset in bytes
} SR_VertexAttribute;

/* Vertex, index and attribute data collection. */
typedef struct {
    double* vertexBuffer;
    size_t* indexBuffer;
    size_t vertexBufferSize;
    size_t indexBufferSize;
    size_t attributesCount;
    SR_VertexAttribute* attributes;
} SR_VertexArray; 

/* Framebuffer */
typedef struct {
    SR_TextureBuffer color;
    SR_TextureBuffer depth;
} SR_FrameBuffer;

#endif // TYPES_H
