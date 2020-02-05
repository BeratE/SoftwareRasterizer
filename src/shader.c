#include <string.h>
#include <matrix.h>
#include "sre/sre.h"

/* Global state */
extern SMOL_Matrix _perspectiveMat;
extern SMOL_Matrix _viewMat;
extern SMOL_Matrix _modelMat;

extern SR_TexBuffer2D _image;
/* ~/Global state/~ */

/* Shader functions */
void vertexShader(size_t count, SR_Vecf *attribs, SR_Vec4f *vPos)
{
    if (count < 3)
	return;
    
    // Vertex Input
    SR_Vec3f aPos = attribs[0].vec3f;
    SR_Vec3f aColor = attribs[1].vec3f;
    SR_Vec2f aUV = attribs[2].vec2f;
    
    SMOL_Matrix p = (SMOL_Matrix){.nRows = 4, .nCols = 1, .fields = (double*)&aPos};
    p.fields[3] = 1.0;
   
    SMOL_Matrix mvp;
    SMOL_MultiplyV(&mvp, 3, &_modelMat, &_viewMat, &_perspectiveMat);

    // Set Vertex Position
    SMOL_Matrix k;
    SMOL_Multiply(&k, &mvp, &p);
    memcpy(vPos, k.fields, sizeof(double)*4);

    // Set Vertex Output
    SR_SetVertexStageOutput(0, (SR_Vecf*)&aColor);
    SR_SetVertexStageOutput(1, (SR_Vecf*)&aUV);

    SMOL_FreeV(2, &k, &mvp);
}

void fragmentShader(size_t count, SR_Vecf *attribs, SR_Vec4f *fColor)
{
    if (count < 2)
	return;
    
    SR_Vec4f aColor = attribs[0].vec4f;
    SR_Vec2f aUV = attribs[1].vec2f;
    uint8_t c[4];
    SR_TexBufferRead(&_image, &c, aUV.x*_image.width, aUV.y*_image.height);
    // uv coordinates in [0, 1]
    fColor->x = c[0]/255.0;
    fColor->y = c[1]/255.0;
    fColor->z = c[2]/255.0;
    fColor->w = c[3]/255.0;
}
/* ~/Shader functions/~ */
