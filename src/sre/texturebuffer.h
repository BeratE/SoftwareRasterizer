#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H
/* Data types and functions for operating on texture buffers. 
 * Memory is stored in an array of size width*height*fsize bytes. 
 * fsize determines the number of components per color entry (in bytes). */

#include "textureformat.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char fsize;
    unsigned char *values;
} TextureBuffer;

TextureBuffer* CreateTextureBuffer(unsigned int width, unsigned int height,
				   unsigned char formatsize);

void ClearTextureBuffer(TextureBuffer *buffer, unsigned char value);

#endif // TEXTUREBUFFER_H
