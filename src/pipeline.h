#ifndef PIPELINE_H
#define PIPELINE_H
/* Functions for accessing state and rendering pipeline. */

enum PRIMITIVE_TYPE {
    POINT, LINE, TRIANGLE
};

// Set Global States
void setViewPort(uint32_t x0, uint32_t y0,
		 uint32_t width, uint32_t height);
void setFrustum(double near, double far, double fov, double aspect);

// Rendering Pipeline
void drawArrays(int primitive_type, double *arraybuffer, size_t buffersize); 

#endif // PIPELINE_H
