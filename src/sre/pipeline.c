#include "pipeline.h"
#include <string.h>
#include <matrix.h>
#include <math.h>
#include "rasterizer.h"

/* ~Global State~ */
static SR_FrameBuffer _framebuffer;
/* ~Global State~ */

/* Static functions */
static void perspectiveDivide(double* vertex)
{
    for (size_t i = 0; i < 4; i++)
	vertex[i] /= vertex[3];
}

static void viewportTransform(double* vertex, size_t *uv)
{
    const size_t w = _framebuffer.colorBuffer.width;
    const size_t h = _framebuffer.colorBuffer.height;
    uv[0] = (size_t)(vertex[0] * w/2 + w/2);
    uv[1] = (size_t)(vertex[1] * h/2 + h/2);
}
/* Static functions */


void SR_Init()
/* Initialize the software rasterization engine. */
{
    SR_SetViewPort(0, 0);
}

void SR_Shutdown()
/* Clean up resources. */
{
    SR_FreeTextureBuffer(&_framebuffer.colorBuffer);
    SR_FreeTextureBuffer(&_framebuffer.depthBuffer);
}

void SR_SetViewPort(int w, int h)
/* Set global viewport state parameters. */
{
    SR_FreeTextureBuffer(&_framebuffer.colorBuffer);
    SR_FreeTextureBuffer(&_framebuffer.depthBuffer);
    _framebuffer.colorBuffer = SR_CreateTextureBuffer(w, h, SR_TEXTURE_FORMAT_RGBA8);
    _framebuffer.depthBuffer = SR_CreateTextureBuffer(w, h, SR_TEXTURE_FORMAT_R8);
}

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask) {
    if (buffermask & SR_COLOR_BUFFER_BIT)
	SR_ClearTextureBuffer(&_framebuffer.colorBuffer, 0.0);
    if (buffermask & SR_DEPTH_BUFFER_BIT)
	SR_ClearTextureBuffer(&_framebuffer.depthBuffer, 0.0);
}

SR_TextureBuffer SR_Blit(enum SR_RENDER_TARGET_BIT bufferbit)
/* Return a copy of a rendertarget of the framebuffer. */
{
    SR_TextureBuffer b = SR_NULL_TEXTUREBUFFER;

    switch (bufferbit) {
    case (SR_COLOR_BUFFER_BIT):
	b = SR_CopyTextureBuffer(&_framebuffer.colorBuffer);
	break;
	
    case (SR_DEPTH_BUFFER_BIT):
	b = SR_CopyTextureBuffer(&_framebuffer.depthBuffer);
	break;
	
    default:
	break;
    }

    return b;
}

void SR_DrawArrays(enum SR_PRIMITIVE_TYPE type, size_t count)
{
    const double input[] = {-0.5, -0.5, 0.0, 1.0,
			    -0.5,  0.5, 0.0, 1.0,
			     0.5, -0.5, 0.0, 1.0,
			     0.5, -0.5, 0.0, 1.0,
			    -0.5,  0.5, 0.0, 1.0,
			     0.5,  0.5, 0.0, 1.0};

    double *data = malloc(sizeof input);
    memcpy(data, input, sizeof input);

    const size_t nCompPerVert = 4;
    const size_t nVertPerPrim = type;
    //const size_t nDataPoints = sizeof(input) / sizeof(input[0]); 
    //const size_t nVertices = nDataPoints / nCompPerVert;
    const size_t nVertices = count;

    double* vp = NULL;
    size_t windowCord[2* nVertPerPrim];
    
    // Per primitive processing
    for (size_t i = 0; i < nVertices; i+=nVertPerPrim) {

	// Clipping
	
	// Perspective divide
	for (size_t k = 0; k < nVertPerPrim; k++) {
	    vp = &data[(i+k)*nCompPerVert];
	    perspectiveDivide(vp);
	}
	
	// Viewport transform
	for (size_t k = 0; k < nVertPerPrim; k++) {
	    vp = &data[(i+k)*nCompPerVert];
	    viewportTransform(vp, &windowCord[k*2]);
	}
	
	SR_Texel texel = (SR_Texel)(SR_RGBA8){.r=255,.g=100,.b = 100,.a=255};
	SR_WriteTriangle(&_framebuffer.colorBuffer, windowCord, &texel);
    }

    free(data);
}
