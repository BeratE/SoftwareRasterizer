#ifndef RASTERIZER_H
#define RASTERIZER_H
/* Methods for rasterizing primitives onto a colorbuffer. */

#include "colorbuffer.h"

void WritePixel(ColorBuffer *buffer,
		uint_fast32_t x, uint_fast32_t y,
		uint8_t b, uint8_t g, uint8_t r, uint8_t a);

void WriteLine(ColorBuffer *buffer,
	       uint_fast32_t x0, uint_fast32_t y0,
	       uint_fast32_t x1, uint_fast32_t y1, uint8_t value);

#endif // RASTERIZER_H
