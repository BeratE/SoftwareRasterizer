#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include <vector>

struct Color {
    uint8_t b, g, r, a;
};

struct ColorBuffer {
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t>buffer;
};

ColorBuffer* CreateColorBuffer(uint32_t width, uint32_t height)
/* Return a new ColorBuffer object in the BGRA8888 Format. */
{
    ColorBuffer *buffer = new ColorBuffer();
    buffer->width = width;
    buffer->height = height;
    buffer->buffer.assign(width * height * 4, 0);
    return buffer;
}

void WritePixel(ColorBuffer *buffer, uint32_t x, uint32_t y, uint8_t b, uint8_t g, uint8_t r, uint8_t a)
/* Writes the desired color values in the x,y coordinates of the color buffer. */
{
    const uint32_t offset = (buffer->width * y + x) * 4;
    buffer->buffer[offset + 0] = b;
    buffer->buffer[offset + 1] = g;
    buffer->buffer[offset + 2] = r;
    buffer->buffer[offset + 3] = a;
}

void WritePixel(ColorBuffer *buffer, uint32_t x, uint32_t y, Color color)
{
    WritePixel(buffer, x, y, color.b, color.g, color.r, color.a);
}

void WritePixel(ColorBuffer *buffer, uint32_t x, uint32_t y, uint8_t value, uint8_t alpha = 255)
{
    WritePixel(buffer, x, y, value, value, value, alpha);
}

void WriteLine(ColorBuffer *buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t value)
/* Bresenheim Midpoint Line Rasterization.  */
{
    // Up - Down Symetry
    if (x1 < x0) {
	x1 = x1 + x0;
	x0 = x1 - x0;
	x1 = x1 - x0;
       
	y1 = y1 + y0;
	y0 = y1 - y0;
	y1 = y1 - y0;
    }
    
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;

    // Left - Right Symetry
    /*int32_t yi = 1;
    if (dy < 0) {
	yi = -1;
	dy = -dy;
    }*/

    // m <= 1
    int32_t d = 2*dy - dx;
    int32_t smallerIncr = 2*dy; // Step into E | N
    int32_t greaterIncr = 2*(dy - dx); // Step into NE
    int32_t smIncX = 1;
    int32_t smIncY = 0;
    int32_t grIncX = 1;
    int32_t grIncY = 1;
    // m > 1
    if ((dy*dy) > (dx*dx)) {
	d = dy - 2*dx;
	smallerIncr = greaterIncr;
	greaterIncr = -2 * dx;
	smIncX = 1;
	smIncY = 1;
	grIncX = 0;
	grIncY = 1;
    }
    
    uint32_t x = x0;
    uint32_t y = y0;

    // Incremental Rasterization
    while (x <= x1 && y <= y1) {
	WritePixel(buffer, x, y, value);
	if (d <= 0) {
	    d += smallerIncr;
	    x += smIncX;
	    y += smIncY;
	}
	else {       
	    d += greaterIncr;
	    x += grIncX;
	    y += grIncY;
	}
    }
}

#endif
