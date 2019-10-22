#ifndef COLORBUFFER_H
#define COLORBUFFER_H

#include <stdint.h>

typedef struct{
    uint8_t b, g, r, a;
} ColorBGRA8;

typedef struct {
    uint_fast32_t width;
    uint_fast32_t height;
    uint8_t *array;
    // Number of Components
    uint_fast8_t format_size; 
} ColorBuffer;


ColorBuffer* CreateColorBuffer(uint_fast32_t width, uint_fast32_t height,
			       uint_fast8_t format_size);

void ClearColorBuffer(ColorBuffer *buffer, uint8_t value);

#endif // COLORBUFFER_H
