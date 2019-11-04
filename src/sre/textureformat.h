#ifndef TEXTUREFORMAT_H
#define TEXTUREFORMAT_H

#include <stdint.h>

/* Avaliable Texture formats defined as sizes (in bytes).*/
#define TEXTURE_FORMAT_R8     1
#define TEXTURE_FORMAT_RG16   2
#define TEXTURE_FORMAT_RGB8   3
#define TEXTURE_FORMAT_RGBA8  4
#define TEXTURE_FORMAT_RGB16  6
#define TEXTURE_FORMAT_RGBA16 8

typedef struct {
    uint8_t r;
} sR8;

typedef struct {
    uint16_t r;
} sR16;

typedef struct {
    uint8_t r, g, b;
} sRGB8;

typedef struct {
    uint8_t r, g, b, a;
} sRGBA8;

typedef struct {
    uint16_t r, g, b;
} sRGB16;

typedef struct {
    uint16_t r, g, b, a;
} sRGBA16;

typedef union {
    sR8     fR8;
    sR16    fR16;
    sRGB8   fRGB8;
    sRGBA8  fRGBA8;
    sRGB16  fRGB16;
    sRGBA16 fRGBA16;
} Texel;

#endif // TEXTUREFORMAT_H
