#include "pipeline.h"
#include "rasterizer.h"
#include "vertexarray.h"
#include <matrix.h>
#include <string.h>
#include <math.h>

/* ~Global State~ */

struct listVAO {
    size_t index;
    SR_VAO* pArrayObject;
    struct listVAO* pNext;
};

static struct listVAO *_listHead;
static SR_VAO *_pCurrVao;
static size_t _nextVaoIndex;

static struct {
    SR_TextureBuffer colorBuffer;
    SR_TextureBuffer depthBuffer;
} _framebuffer;

/* ~/Global State/~ */

/* ~Static functions~ */

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

/* ~/Static functions/~ */


void SR_Init()
/* Initialize the software rasterization engine. */
{
    SR_SetViewPort(0, 0);
    _listHead = malloc(sizeof(struct listVAO));
    _listHead->index = 0;
    _listHead->pNext = NULL;
    _listHead->pArrayObject = NULL;
    _pCurrVao = NULL;
    _nextVaoIndex = 0;
}

void SR_Shutdown()
/* Clean up resources. */
{
    SR_FreeTextureBuffer(&_framebuffer.colorBuffer);
    SR_FreeTextureBuffer(&_framebuffer.depthBuffer);

    // Free Vertex Array Data
    struct listVAO *listp = _listHead;
    while (listp != NULL) {
	if (listp->pArrayObject != NULL) {
	    free(listp->pArrayObject->indexBuffer);
	    free(listp->pArrayObject->vertexBuffer);
	    free(listp->pArrayObject->vertAttribs);
	    free(listp->pArrayObject);
	    listp->pArrayObject = NULL;
	}
	struct listVAO *next = listp->pNext;
	free(listp);
	listp = next;
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

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask)
/* Clear the target buffer with zero-values. */
{
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
    struct listVAO* listp = _listHead;
    while ((listp->pNext != NULL) && (listp = listp->pNext));

    listp->index = ++_nextVaoIndex;
    
    listp->pArrayObject = malloc(sizeof(SR_VAO));
    listp->pArrayObject->indexBuffer = NULL;
    listp->pArrayObject->vertexBuffer = NULL;
    listp->pArrayObject->vertAttribs = NULL;
    listp->pArrayObject->vertAttribCount = 0;

    listp->pNext = malloc(sizeof(struct listVAO));
    listp->pNext->pArrayObject = NULL;
    listp->pNext->pNext = NULL;

    return listp->index;
}

void SR_BindVertexArray(size_t handle)
{
    struct listVAO* listp = _listHead;
    while ((listp != NULL) && (listp->index != handle) && (listp = listp->pNext));
    if (listp != NULL)
	_pCurrVao = listp->pArrayObject;
    else
	_pCurrVao = NULL;
}

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size)
{
    if (_pCurrVao == NULL)
	return;

    switch (target) {
    case SR_VERTEX_BUFFER:
	_pCurrVao->vertexBuffer = malloc(size);
	memcpy(_pCurrVao->vertexBuffer, data, size);
	_pCurrVao->vertexBufferSize = size;
	break;
	
    case SR_INDEX_BUFFER:
	_pCurrVao->indexBuffer = malloc(size);
	memcpy(_pCurrVao->indexBuffer, data, size);
	_pCurrVao->indexBufferSize = size;
	break;
	
    default:
	break;
    }
}

void SR_SetVertexAttributeCount(size_t count)
{
    if (_pCurrVao == NULL)
	return;
    if (_pCurrVao->vertAttribs != NULL) {
	free(_pCurrVao->vertAttribs);
    }
    _pCurrVao->vertAttribs = malloc(sizeof(SR_VertAttrib)*count);
    _pCurrVao->vertAttribCount = count;
}

void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset)
{
    if (_pCurrVao == NULL || index > _pCurrVao->vertAttribCount)
	return;

    _pCurrVao->vertAttribs[index] = (SR_VertAttrib) {
        .count = count,
	.stride=stride,
	.offset=offset
    };
}

void SR_DrawArrays(enum SR_PRIMITIVE_TYPE type, size_t count, size_t startindex)
{
    if (_pCurrVao == NULL ||
	_pCurrVao->vertexBuffer == NULL ||
	_pCurrVao->indexBuffer == NULL)
	return;

    double *vertexdata = malloc(_pCurrVao->vertexBufferSize);
    memcpy(vertexdata, _pCurrVao->vertexBuffer, _pCurrVao->vertexBufferSize);

    const size_t nCompPerVert = 4;
    const size_t nVertPerPrim = type;
    //const size_t nDataPoints = sizeof(input) / sizeof(input[0]); 
    //const size_t nVertices = nDataPoints / nCompPerVert;

    double* pVertex = NULL;
    size_t windowCord[2* nVertPerPrim];
    
    // Per primitive processing
    size_t index = 0;
    for (size_t i = startindex; i < (startindex+count); i+=nVertPerPrim) {
	// Clipping
	
	// Perspective divide
	for (size_t k = i; k < i+nVertPerPrim; k++) {
	    index = _pCurrVao->indexBuffer[k];
	    pVertex = &vertexdata[index*nCompPerVert];
	    perspectiveDivide(pVertex);
	}
	
	// Viewport transform
	for (size_t k = i; k < i+nVertPerPrim; k++) {
	    index = _pCurrVao->indexBuffer[k];
	    pVertex = &vertexdata[index*nCompPerVert];
	    viewportTransform(pVertex, &windowCord[(k-i)*2]);
	}
	
	SR_Texel texel = (SR_Texel)(SR_RGBA8){.r=255,.g=100,.b = 100,.a=255};
	SR_WriteTriangle(&_framebuffer.colorBuffer, windowCord, &texel);
    }

    free(vertexdata);
}
