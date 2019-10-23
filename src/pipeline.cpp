#include "pipeline.h"

static struct ViewPort {
    uint32_t x0, y0, width, height;
} _viewPort;

static struct Frustum {
    double near, far, fov, aspect;
} _frustum;

void setViewPort(uint32_t x0, uint32_t y0,
		 uint32_t width, uint32_t height)
{
    _viewPort.x0 = x0;
    _viewPort.y0 = y0;
    _viewPort.width = width;
    _viewPort.height = height;
}

void setFrustum(double near, double far, double fov, double aspect)
{
    _frustum.near = near;
    _frustum.far = far;
    _frustum.fov = fov;
    _frustum.aspect = aspect;
}
    
