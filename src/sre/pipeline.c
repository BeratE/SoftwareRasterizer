#include "pipeline.h"
#include "rasterizer.h"
#include "vertexarray.h"
#include <matrix.h>
#include <string.h>
#include <math.h>

/* ~Global State~ */

struct vaoList {
    size_t index;
    SR_VAO* array;
    struct vaoList* next;
};

static struct vaoList *_vaoListHead;
static SR_VAO *_vaop;
static size_t _nextVaoIndex;


static struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} _framebuffer;

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
    _vaoListHead = NULL;
    _nextVaoIndex = 1;
    _vaop = NULL;
}

void SR_Shutdown()
/* Clean up resources. */
{
    SR_FreeTextureBuffer(&_framebuffer.colorBuffer);
    SR_FreeTextureBuffer(&_framebuffer.depthBuffer);

    struct vaoList *listp = _vaoListHead;
    while (listp != NULL) {
	if (listp->array != NULL) {
	    free(listp->array->indexBuffer);
	    free(listp->array->vertexBuffer);
	    free(listp->array->vertAttribs);
	    free(listp->array);
	    listp->array = NULL;
	}
	listp = listp->next;
    }
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

void SR_Blit(enum SR_RENDER_TARGET_BIT bufferbit, SR_TextureBuffer *buffer)
/* Return a copy of a rendertarget of the framebuffer. */
{
    switch (bufferbit) {
    case (SR_COLOR_BUFFER_BIT):
	*buffer = SR_CopyTextureBuffer(&_framebuffer.colorBuffer);
	break;
	
    case (SR_DEPTH_BUFFER_BIT):
	*buffer = SR_CopyTextureBuffer(&_framebuffer.depthBuffer);
	break;
	
    default:
	break;
    }
}

size_t SR_GenVertexArray()
{
    struct vaoList* listp = _vaoListHead;
    while ((listp != NULL) && (listp = listp->next));

    listp = malloc(sizeof(struct vaoList));
    listp->index = ++_nextVaoIndex;
    listp->next = NULL;
    
    listp->array = malloc(sizeof(SR_VAO));
    listp->array->indexBuffer = NULL;
    listp->array->vertexBuffer = NULL;
    listp->array->vertAttribs = NULL;
    listp->array->vertAttribCount = 0;

    return listp->index;
}

void SR_BindVertexArray(size_t handle)
{
    struct vaoList* listp = _vaoListHead;
    while ((listp != NULL) && (listp->index != handle) && (listp = listp->next));
    if (listp != NULL)
	_vaop = listp->array;
    else
	_vaop = NULL;
}

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size)
{
    if (_vaop == NULL)
	return;

    switch (target) {
    case SR_VERTEX_BUFFER:
	_vaop->vertexBuffer = malloc(sizeof(double)*size);
	memcpy(_vaop->vertexBuffer, data, sizeof(double)*size);
	break;
	
    case SR_INDEX_BUFFER:
	_vaop->indexBuffer = malloc(sizeof(size_t)*size);
	memcpy(_vaop->indexBuffer, data, sizeof(size_t)*size);
	break;
	
    default:
	break;
    }
}

void SR_SetVertexAttributeCount(size_t count)
{
    if (_vaop == NULL)
	return;
    if (_vaop->vertAttribs != NULL) {
	free(_vaop->vertAttribs);
    }
    _vaop->vertAttribs = malloc(sizeof(SR_VertAttrib)*count);
    _vaop->vertAttribCount = count;
	
}

void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset)
{
    if (_vaop == NULL || index > _vaop->vertAttribCount)
	return;

    _vaop->vertAttribs[index] = (SR_VertAttrib) {
        .count = count,
	.stride=stride,
	.offset=offset
    };
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
