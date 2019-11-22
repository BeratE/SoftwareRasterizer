#include "pipeline.h"
#include <string.h>
#include <matrix.h>
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
    uv[0] = vertex[0] * w/2 + w/2;
    uv[1] = vertex[1] * h/2 + h/2;
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
    _framebuffer.colorBuffer = SR_CreateTextureBuffer(w, h, SR_TEXTURE_FORMAT_RGBA8);
    _framebuffer.depthBuffer = SR_CreateTextureBuffer(w, h, SR_TEXTURE_FORMAT_R8);
}

SR_FrameBuffer SR_FetchFrameBuffer()
{
    return _framebuffer;
}

void SR_DrawCube()
{
    SR_ClearTextureBuffer(&_framebuffer.colorBuffer, 0);
    SR_ClearTextureBuffer(&_framebuffer.depthBuffer, 0);
    
    const double input[] = {-0.5, -0.5, 0.0, 1.0,
			    0.0,  0.5, 0.0, 1.0,
			    0.5, -0.5, 0.0, 1.0};

    double *data = malloc(sizeof input);
    memcpy(data, input, sizeof input);
       	
    const size_t nVertComp = 4;
    const size_t nDataComp = sizeof(input) / sizeof(input[0]); 
    const size_t nVertices = nDataComp / nVertComp;

    for (size_t i = 0; i < nVertices; i+=nVertComp) {
	size_t winPos[6];
	for (int k = 0; k < 3; k++) {
	    double *vertex = &data[(i+k)*nVertComp];
	    perspectiveDivide(vertex);
	    viewportTransform(vertex, &winPos[k*2]);
	}
	SR_Texel texel = (SR_Texel)(SR_RGBA8){.r=255,.g=100,.b = 100,.a=255};
	SR_WriteTriangle(&_framebuffer.colorBuffer, winPos, &texel);
    }

    free(data);
}
