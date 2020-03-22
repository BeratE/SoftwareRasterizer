/* Common and shared definitions. */

#ifndef SREDEFS_H
#define SREDEFS_H

#include <stdint.h>
#include <stdlib.h>

/* Macros */
#define MIN3(a, b, c) (a < b) ? (a < c ? a : c) : (b < c ? b : c)
#define MAX3(a, b, c) (a > b) ? (a > c ? a : c) : (b > c ? b : c)
#define CLAMP(x, a, b) (x < b) ? ((x < a) ? a : x) : b // clamp x between (a, b)
/* ~/Macros/~ */

// Common Constants and Enumerations
enum SR_PRIMITIVE_TYPE {
    SR_PT_POINTS = 1,
    SR_PT_LINES,
    SR_PT_TRIANGLES
};

enum SR_BUFFER_TYPE {
    SR_BT_VERTEX_BUFFER,
    SR_BT_INDEX_BUFFER
};

enum SR_SHADER_TYPE{
    SR_ST_VERTEX_SHADER,
    SR_ST_FRAGMENT_SHADER
};

enum SR_RENDER_TARGET_BIT {
    SR_RTB_COLOR_BUFFER_BIT = 0x1,
    SR_RTB_DEPTH_BUFFER_BIT = 0x2
};


// Texture Format
enum SR_TEXTURE_FORMAT_MASK {
    SR_TF_MASK_TYPE = 0xF000,
    SR_TF_MASK_NCOMPS = 0x00F0,
    SR_TF_MASK_NBYTES = 0x000F,
};

enum SR_TEXTURE_FORMAT_TYPE {
    SR_TF_TYPE_UINT = 0x0000,
    SR_TF_TYPE_FLOAT = 0x1000,
};

enum SR_TEXTURE_FORMAT {
    /* Bytecoded Format  = [ Type | N Comp. | 0 | Num Bytes] */
    // Composite Integer Types - 0
    SR_TEX_FORMAT_R8     = SR_TF_TYPE_UINT | (1 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RG8    = SR_TF_TYPE_UINT | (2 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RGB8   = SR_TF_TYPE_UINT | (3 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_RGBA8  = SR_TF_TYPE_UINT | (4 << 4) | sizeof(uint8_t),
    SR_TEX_FORMAT_R16    = SR_TF_TYPE_UINT | (1 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RG16   = SR_TF_TYPE_UINT | (2 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RGB16  = SR_TF_TYPE_UINT | (3 << 4) | sizeof(uint16_t),
    SR_TEX_FORMAT_RGBA16 = SR_TF_TYPE_UINT | (4 << 4) | sizeof(uint16_t),
    // Floating Point Types - 1
    SR_TEX_FORMAT_F32 = SR_TF_TYPE_FLOAT | (1 << 4) | sizeof(float),
};


#endif // SREDEFS_H
