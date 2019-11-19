#include "pipeline.h"
#include "string.h"

/* ~Global State~ */
static SR_Framebuffer _framebuffer;
/* ~Global State~ */

/* Static functions */

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

void SR_DrawCube()
{
    double vert[] = {-0.5, -0.5, 0.0, 1.0,
		      0.0,  0.5, 0.0, 1.0,
		     0.5, -0.5, 0.0, 1.0};
    
}
