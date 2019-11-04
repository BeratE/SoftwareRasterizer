#ifndef PIPELINE_H
#define PIPELINE_H
/* Functions for accessing state and rendering pipeline. */

#include <stdlib.h>
#include "texturebuffer.h"

enum PRIMITIVE_TYPE {
    POINT, LINE, TRIANGLE
};

// Set Global States
void setViewPort(unsigned int x0, unsigned int y0,
		 unsigned int width, unsigned int height);
void setFrustum(double near, double far, double fov, double aspect);

// Rendering Pipeline
void drawArrays(int primitive, const double *arraybuffer,
		size_t buffer_size, size_t buffer_offset, size_t stride); 

#endif // PIPELINE_H
