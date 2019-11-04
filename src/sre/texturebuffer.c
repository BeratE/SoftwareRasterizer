#include "texturebuffer.h"
#include <stdlib.h>
#include <string.h>

TextureBuffer* CreateTextureBuffer(unsigned int width, unsigned int height,
				   unsigned char formatsize)
/* Return a new ColorBuffer with allocated memory. */
{
    TextureBuffer *buffer = malloc(sizeof(TextureBuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->fsize = formatsize;
    size_t size = width * height * formatsize;
    buffer->values = malloc(size);
    memset(buffer->values, 0, size);
    return buffer;
}

void ClearTextureBuffer(TextureBuffer *buffer, unsigned char value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->values, value, buffer->width*buffer->height*buffer->fsize);
}
