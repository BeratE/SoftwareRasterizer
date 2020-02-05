#include "sre.h"
#include <string.h>
#include <math.h>

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

void SR_TexBufferClear(SR_TexBuffer2D *buffer, const void* value)
/* Clears the elements of the color buffer array with the given value. */
{
    for (size_t i = 0; i < buffer->width*buffer->height; i++) {
	memcpy(&buffer->values[i*buffer->fsize], value, buffer->fsize);
    }
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

void SR_TexBufferSample(SR_TexBuffer2D *buffer, void* outValue, float x, float y)
/* Sample position with bilinear interpolation. */
{/*
    // Sample positions
    size_t x0 = (size_t)x;
    size_t y0 = (size_t)y;
    size_t x1 = x0 + 1;
    size_t y1 = y0 + 1;
    x0 = CLAMP(x0, 0, buffer->width);
    y0 = CLAMP(y0, 0, buffer->height);
    x1 = CLAMP(x1, 0, buffer->width);
    y1 = CLAMP(y1, 0, buffer->height);

    // Samples
    uint8_t* samples = malloc(4 * buffer->fsize);
    SR_TexBufferRead(buffer, &samples[0], x0, y0);
    SR_TexBufferRead(buffer, &samples[1], x1, y0);
    SR_TexBufferRead(buffer, &samples[2], x0, y1);
    SR_TexBufferRead(buffer, &samples[3], x1, y1);

    // Interpolate in x direction
    float tx = x - floor(x); // [0, 1]
    float sx1 = (1-tx)*samples[0] + tx*samples[1];
    float sx2 = (1-tx)*samples[2] + tx*samples[3];

    // Interpolate in y direction
    float ty = y - floor(y); // [0, 1]
    float res = (1-ty)*sx1 + ty*sx2;
    
    
    free(samples);
 */
}
    

void SR_TexBufferWrite(SR_TexBuffer2D *buffer, const void *value, size_t x, size_t y)
/* Write the given value (of format size) into the texture buffer at position (x,y). */
{
    const size_t offset = (y * buffer->width + x) * buffer->fsize;
    memcpy(&buffer->values[offset], value, buffer->fsize);
}
