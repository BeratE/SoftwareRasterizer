#include "sre.h"
#include <string.h>

SR_TexBuffer2D SR_TexBufferCreate(size_t width, size_t height, uint8_t formatsize)
/* Return a new ColorBuffer with allocated memory. */
{
    SR_TexBuffer2D b = (SR_TexBuffer2D){
        .fsize = formatsize,
	.width = width,
	.height = height
    };
    const size_t size = width * height * formatsize;
    b.values = malloc(size);
    memset(b.values, 0, size);
    return b;
}

SR_TexBuffer2D SR_TexBufferCopy(const SR_TexBuffer2D* buffer)
/* Copy the contents of the given texture buffer into a newly allocated buffer. */
{
    SR_TexBuffer2D tex = SR_TexBufferCreate(buffer->width, buffer->height,
					    buffer->fsize);
    memcpy(tex.values, buffer->values, tex.width * tex.height * tex.fsize);
    return tex;
}

void SR_TexBufferFree(SR_TexBuffer2D *buffer)
/* Free allocated memory for the texture buffer. */
{
    if (buffer->values != NULL) {
	free(buffer->values);
    }
    *buffer = SR_NULL_TEXBUFFER;
}

void SR_TexBufferClear(SR_TexBuffer2D *buffer, uint8_t value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->values, value, buffer->width*buffer->height*buffer->fsize);
}

size_t SR_TexBufferSize(SR_TexBuffer2D *buffer)
/* Return the size of the texture buffer in size. */
{
    return buffer->width*buffer->height*buffer->fsize;
}

void SR_TexBufferRead(SR_TexBuffer2D *buffer, void* outValue, size_t x, size_t y)
/* Read the value at psoition (x,y) into the out value. */
{
    const size_t offset = (y * buffer->width + x) * buffer->fsize;
    memcpy(outValue, &buffer->values[offset], buffer->fsize);
}

void SR_TexBufferWrite(SR_TexBuffer2D *buffer, void *value, size_t x, size_t y)
/* Write the given value (of format size) into the texture buffer at position (x,y). */
{
    const size_t offset = (y * buffer->width + x) * buffer->fsize;
    memcpy(&buffer->values[offset], value, buffer->fsize);
}
