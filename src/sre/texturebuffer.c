#include "texturebuffer.h"
#include <stdlib.h>
#include <string.h>

SR_TextureBuffer SR_CreateTextureBuffer(unsigned int width, unsigned int height,
					 unsigned char formatsize)
/* Return a new ColorBuffer with allocated memory. */
{
    SR_TextureBuffer buffer;
    buffer.width = width;
    buffer.height = height;
    buffer.fsize = formatsize;
    size_t size = width * height * formatsize;
    buffer.values = malloc(size);
    memset(buffer.values, 0, size);
    return buffer;
}

void SR_FreeTextureBuffer(SR_TextureBuffer *buffer)
/* Free allocated memory for the texture buffer. */
{
    if (buffer->values != NULL) {
	free(buffer->values);
    }
    *buffer = SR_NULLTEXTUREBUFFER;
}

void SR_ClearTextureBuffer(SR_TextureBuffer *buffer, unsigned char value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->values, value, buffer->width*buffer->height*buffer->fsize);
}
