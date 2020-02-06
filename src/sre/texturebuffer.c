#include "sre.h"
#include <string.h>
#include <math.h>

SR_TexBuffer2D SR_TexBufferCreate(size_t width, size_t height, uint16_t format)
/* Return a new ColorBuffer with allocated memory. */
{
    SR_TexBuffer2D b = (SR_TexBuffer2D){
        .format = format,
	.width = width,
	.height = height
    };
    const size_t size = SR_TexBufferSize(&b);
    b.values = malloc(size);
    memset(b.values, 0, size);
    return b;
}

SR_TexBuffer2D SR_TexBufferCopy(const SR_TexBuffer2D* buffer)
/* Copy the contents of the given texture buffer into a newly allocated buffer. */
{
    SR_TexBuffer2D tex = SR_TexBufferCreate(buffer->width, buffer->height,
					    buffer->format);
    memcpy(tex.values, buffer->values, SR_TexBufferSize(buffer));
    return tex;
}

void SR_TexBufferRead(const SR_TexBuffer2D *buffer, void* outValue, size_t x, size_t y)
/* Read the value at psoition (x,y) into the out value. */
{
    const size_t fsize = SR_TexBufferFormatSize(buffer);
    const size_t offset = (y * buffer->width + x) * fsize;
    memcpy(outValue, &buffer->values[offset], fsize);
}

void SR_TexBufferSample(const SR_TexBuffer2D *buffer, void* outValue, double x, double y)
/* Sample position with bilinear interpolation. */
{
    // Sample positions
    int x0 = (size_t)x;
    int y0 = (size_t)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    x0 = CLAMP(x0, 0, (int)buffer->width);
    y0 = CLAMP(y0, 0, (int)buffer->height);
    x1 = CLAMP(x1, 0, (int)buffer->width);
    y1 = CLAMP(y1, 0, (int)buffer->height);

    // Samples
    const uint16_t type = SR_TexBufferFormatType(buffer);
    const uint16_t ncomp = SR_TexBufferFormatNComps(buffer);
    const uint16_t nbytes = SR_TexBufferFormatNBytes(buffer);
    const size_t fsize = SR_TexBufferFormatSize(buffer);
    
    uint8_t* samples = malloc(4 * fsize);
    SR_TexBufferRead(buffer, &samples[0*fsize], x0, y0);
    SR_TexBufferRead(buffer, &samples[1*fsize], x1, y0);
    SR_TexBufferRead(buffer, &samples[2*fsize], x0, y1);
    SR_TexBufferRead(buffer, &samples[3*fsize], x1, y1);

    // Interpolation
    const double tx = x - floor(x); // [0, 1]
    const double ty = y - floor(y); // [0, 1]
    
    switch(type) {
    case SR_TEX_TYPE_UINT:
	for (uint8_t i = 0; i < ncomp; i++) {
	    // Collect values of sample components
	    size_t values[4] = {0};
	    for (int j = 0; j < 4; j++)
		memcpy(&values[j], &samples[(j*fsize)+(i*nbytes)], nbytes);
	    
	    // Interpolate in x direction
	    size_t sx1 = (1-tx)*values[0] + tx*values[1];
	    size_t sx2 = (1-tx)*values[2] + tx*values[3];
	    
	    // Interpolate in y direction
	    size_t sy = (1-ty)*sx1 + ty*sx2;
	    memcpy(&((uint8_t*)outValue)[i*nbytes], &sy, nbytes);
	}
	break;
	
    case SR_TEX_TYPE_FLOAT:
	for (uint16_t i = 0; i < ncomp; i++) {
	    // Interpolate in x direction
	    float sx1 = (1-tx)*((float*)samples)[(0*ncomp)+i] + tx*((float*)samples)[(1*ncomp)+i];
	    float sx2 = (1-tx)*((float*)samples)[(2*ncomp)+i] + tx*((float*)samples)[(3*ncomp)+i];
	    // Interpolate in y direction
	    ((float*)outValue)[i] = (1-ty)*sx1 + ty*sx2;
	}
	break;
    };
    
    free(samples);
}

void SR_TexBufferWrite(SR_TexBuffer2D *buffer, const void *value, size_t x, size_t y)
/* Write the given value (of format size) into the texture buffer at position (x,y). */
{
    const size_t fsize = SR_TexBufferFormatSize(buffer);
    const size_t offset = (y * buffer->width + x) * fsize;
    memcpy(&buffer->values[offset], value, fsize);
}

void SR_TexBufferClear(SR_TexBuffer2D *buffer, const void* value)
/* Clears the elements of the color buffer array with the given value. */
{
    const size_t fsize = SR_TexBufferFormatSize(buffer);
    for (size_t i = 0; i < buffer->width*buffer->height; i++) {
	memcpy(&buffer->values[i*fsize], value, fsize);
    }
}

void SR_TexBufferFree(SR_TexBuffer2D *buffer)
/* Free allocated memory for the texture buffer. */
{
    if (buffer->values != NULL) {
	free(buffer->values);
    }
    *buffer = SR_NULL_TEXBUFFER;
}

size_t SR_TexBufferSize(const SR_TexBuffer2D *buffer)
/* Return the size of the texture buffer in bytes. */
{
    return buffer->width*buffer->height*SR_TexBufferFormatSize(buffer);
}

size_t SR_TexBufferFormatSize(const SR_TexBuffer2D *buffer)
/* Return the size of the textures format in bytes. */
{
    return SR_TexBufferFormatNBytes(buffer) * SR_TexBufferFormatNComps(buffer);
}

uint16_t SR_TexBufferFormatType(const SR_TexBuffer2D *buffer)
/* Return the texture buffer format type. */
{
    return (buffer->format & SR_TEX_FMTM_TYPE);
}

uint16_t SR_TexBufferFormatNComps(const SR_TexBuffer2D *buffer)
/* Return the number of components of the texture buffer formats. */
{
    return (buffer->format & SR_TEX_FMTM_NCOMPS) >> 4;
}

uint16_t SR_TexBufferFormatNBytes(const SR_TexBuffer2D *buffer)
/* Return the number of bytes per components of the texture buffer format. */
{
    return (buffer->format & SR_TEX_FMTM_NBYTES);
}
