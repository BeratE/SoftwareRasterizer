#include "texturebuffer.h"
#include <stdlib.h>
#include <string.h>

SR_TextureBuffer SR_CreateTextureBuffer(unsigned int width, unsigned int height,
					 unsigned char formatsize)
/* Return a new ColorBuffer with allocated memory. */
{
    SR_TextureBuffer b = (SR_TextureBuffer){
        .fsize = formatsize,
	.width = width,
	.height = height
    };
    const size_t size = width * height * formatsize;
    b.values = malloc(size);
    memset(b.values, 0, size);
    return b;
}

SR_TextureBuffer SR_CopyTextureBuffer(const SR_TextureBuffer* buffer)
{
    SR_TextureBuffer b = (SR_TextureBuffer){
        .fsize = buffer->fsize,
	.width = buffer->width,
	.height = buffer->height
    };

    const size_t size = b.width * b.height * b.fsize;
    b.values = malloc(size);
    memcpy(b.values, buffer->values, size);
    return b;
}

void SR_FreeTextureBuffer(SR_TextureBuffer *buffer)
/* Free allocated memory for the texture buffer. */
{
    if (buffer->values != NULL) {
	free(buffer->values);
    }
    *buffer = SR_NULL_TEXTUREBUFFER;
}

void SR_ClearTextureBuffer(SR_TextureBuffer *buffer, unsigned char value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->values, value, buffer->width*buffer->height*buffer->fsize);
}
