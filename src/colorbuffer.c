#include "colorbuffer.h"
#include <stdlib.h>
#include <string.h>

ColorBuffer* CreateColorBuffer(uint_fast32_t width, uint_fast32_t height, uint_fast8_t format_size)
/* Return a new ColorBuffer with allocated memory. */
{
    ColorBuffer *buffer = malloc(sizeof(ColorBuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->format_size = format_size;
    size_t size = sizeof(uint8_t)*width*height*format_size;
    buffer->array = malloc(size);
    memset(buffer->array, 0, size);
    return buffer;
}

void ClearColorBuffer(ColorBuffer *buffer, uint8_t value)
/* Clears the elements of the color buffer array with the given value. */
{
    memset(buffer->array, value,
	   sizeof(uint8_t)*buffer->width*buffer->height*buffer->format_size);
}
