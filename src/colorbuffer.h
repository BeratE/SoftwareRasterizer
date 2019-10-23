#ifndef COLORBUFFER_H
#define COLORBUFFER_H
/* Data Types and Functions for operating on color buffers. 
 * Memory is stored in an array of size  width*height*format_size bytes. 
 * format_size determines the number of components per color entry. */

#include <stdint.h>

typedef struct {
    uint_fast32_t width;
    uint_fast32_t height;
    uint8_t *array;
    // Number of Components
    uint_fast8_t format_size; 
} ColorBuffer;

typedef struct{
    uint8_t b, g, r, a;
} ColorBGRA8;

ColorBuffer* CreateColorBuffer(uint_fast32_t width, uint_fast32_t height,
			       uint_fast8_t format_size);

void ClearColorBuffer(ColorBuffer *buffer, uint8_t value);

#endif // COLORBUFFER_H
