#include "pipeline.h"

/* ~Global State~ */
static struct ViewPort {
    unsigned int x0, y0, width, height;
} _viewPort;

static struct Frustum {
    double near, far, fov, aspect;
} _frustum;
/* ~Global State~ */


void setViewPort(unsigned int x0, unsigned int y0,
		 unsigned int width, unsigned int height)
/* Set global viewport state parameters.  */
{
    _viewPort.x0 = x0;
    _viewPort.y0 = y0;
    _viewPort.width = width;
    _viewPort.height = height;
}

void setFrustum(double near, double far, double fov, double aspect)
/* Set global frustum state parameters. */
{
    _frustum.near = near;
    _frustum.far = far;
    _frustum.fov = fov;
    _frustum.aspect = aspect;
}

void drawArrays(int primitive, const double *arraybuffer,
		size_t buffer_size, size_t buffer_offset, size_t stride)
{
    
}
