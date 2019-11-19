#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H
/* Data types and functions for operating on texture buffers. 
 * Memory is stored in an array of size width*height*fsize bytes. 
 * fsize determines the number of components per color entry (in bytes). */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char fsize;
    unsigned char *values;
} SR_TextureBuffer;

#define SR_NULLTEXTUREBUFFER (SR_TextureBuffer){.width=0,.height=0,.fsize=0,.values=NULL}

/* Texture Format */
// Avaliable Texture formats defined as sizes (in bytes).
#define SR_TEXTURE_FORMAT_R8     1
#define SR_TEXTURE_FORMAT_RG16   2
#define SR_TEXTURE_FORMAT_RGB8   3
#define SR_TEXTURE_FORMAT_RGBA8  4
#define SR_TEXTURE_FORMAT_RGB16  6
#define SR_TEXTURE_FORMAT_RGBA16 8

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

/* Functions on Texturebuffers */
SR_TextureBuffer SR_CreateTextureBuffer(unsigned int width, unsigned int height,
					unsigned char formatsize);

void SR_FreeTextureBuffer(SR_TextureBuffer *buffer);

void SR_ClearTextureBuffer(SR_TextureBuffer *buffer, unsigned char value);

#endif // TEXTUREBUFFER_H
