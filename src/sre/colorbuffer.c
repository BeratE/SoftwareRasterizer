#include "colorbuffer.h"
#include <stdlib.h>
#include <string.h>

ColorBuffer* CreateColorBuffer(unsigned int width, unsigned int height,
			       unsigned char format_size)
/* Return a new ColorBuffer with allocated memory. */
{
    ColorBuffer *buffer = malloc(sizeof(ColorBuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->format_size = format_size;
    size_t size = width * height * format_size;
    buffer->array = malloc(size);
    memset(buffer->array, 0, size);
    return buffer;
}

void ClearColorBuffer(ColorBuffer *buffer, unsigned char value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->array, value, buffer->width*buffer->height*buffer->format_size);
}
