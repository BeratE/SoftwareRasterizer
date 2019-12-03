#include "pipeline.h"
#include "rasterizer.h"
#include <matrix.h>
#include <string.h>
#include <math.h>

/* ~Global State~ */

static SR_FrameBuffer _framebuffer;

static size_t _nextListIndex;
static struct listVAO {
    size_t index;
    SR_VertexArray* pArrayObject;
    struct listVAO* pNext;
} *_listHead;
static SR_VertexArray *_pCurrVAO;

static SR_Shader _cbVertexShader;
static SR_Shader _cbFragmentShader;

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
    _listHead = NULL;
    _pCurrVAO = NULL;
    _nextListIndex = 0;
}

void SR_Shutdown()
/* Clean up resources. */
{
    // Free Framebuffer
    SR_FreeTextureBuffer(&_framebuffer.colorBuffer);
    SR_FreeTextureBuffer(&_framebuffer.depthBuffer);

    // Free Vertex Array Data
    struct listVAO *listp = _listHead;
    while (listp != NULL) {
	if (listp->pArrayObject != NULL) {
	    free(listp->pArrayObject->indexBuffer);
	    free(listp->pArrayObject->vertexBuffer);
	    free(listp->pArrayObject->attributes);
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

SR_FrameBuffer SR_GetFrameBuffer()
/* Temporary solution. */
{
    return _framebuffer;
}

size_t SR_GenVertexArray()
/* Generate a new vertex array object and return the object handle. */
{
    size_t returnindex;
    if (_listHead == NULL) {
	_listHead = malloc(sizeof(struct listVAO));
	_listHead->index = ++_nextListIndex;
	_listHead->pNext = NULL;
	
	_listHead->pArrayObject = malloc(sizeof(SR_VertexArray));
	_listHead->pArrayObject->indexBuffer = NULL;
	_listHead->pArrayObject->vertexBuffer = NULL;
	_listHead->pArrayObject->attributes = NULL;
	_listHead->pArrayObject->attributesCount = 0;

	returnindex = _listHead->index;
    }
    else {
	struct listVAO* listp = _listHead;
	while ((listp->pNext != NULL) && (listp = listp->pNext));
    
	listp->pNext = malloc(sizeof(struct listVAO));
	listp->pNext->index = ++_nextListIndex;
	listp->pNext->pNext = NULL;
    
	listp->pNext->pArrayObject = malloc(sizeof(SR_VertexArray));
	listp->pNext->pArrayObject->indexBuffer = NULL;
	listp->pNext->pArrayObject->vertexBuffer = NULL;
	listp->pNext->pArrayObject->attributes = NULL;
	listp->pNext->pArrayObject->attributesCount = 0;

	returnindex = listp->pNext->index;
    }
    return returnindex;
}

void SR_DestroyVertexArray(size_t handle)
{
    if (_listHead == NULL || handle > _nextListIndex)
	return;
    
    struct listVAO* listp = _listHead;
    struct listVAO* nextp = _listHead->pNext;
    if (_listHead->index == handle) {
	_listHead = nextp;
	free(listp->pArrayObject->indexBuffer);
	free(listp->pArrayObject->vertexBuffer);
	free(listp->pArrayObject->attributes);
	free(listp->pArrayObject);
	free(listp);
    }
    else {
	while (nextp != NULL && nextp->index != handle) {
	    listp = nextp;
	    nextp = nextp->pNext;
	}
    }
    
}

void SR_BindVertexArray(size_t handle)
/* Set the vertex array object identified by handle as the currently bound vao. */
{
    struct listVAO* listp = _listHead;
    while ((listp != NULL) && (listp->index != handle) && (listp = listp->pNext));
    if (listp != NULL)
	_pCurrVAO = listp->pArrayObject;
    else
	_pCurrVAO = NULL;
}

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void* data, size_t size)
/* Set the vertex buffer data of the currently bound vao. */
{
    if (_pCurrVAO == NULL)
	return;

    switch (target) {
    case SR_VERTEX_BUFFER:
	_pCurrVAO->vertexBuffer = malloc(size);
	memcpy(_pCurrVAO->vertexBuffer, data, size);
	_pCurrVAO->vertexBufferSize = size;
	break;
	
    case SR_INDEX_BUFFER:
	_pCurrVAO->indexBuffer = malloc(size);
	memcpy(_pCurrVAO->indexBuffer, data, size);
	_pCurrVAO->indexBufferSize = size;
	break;
	
    default:
	break;
    }
}

void SR_SetVertexAttributeCount(size_t count)
/* Set the number of vertex attributes passed in the currently bound vao. */
{
    if (_pCurrVAO == NULL)
	return;
    if (_pCurrVAO->attributes != NULL) {
	free(_pCurrVAO->attributes);
    }
    _pCurrVAO->attributes = malloc(sizeof(SR_VertexAttribute)*count);
    _pCurrVAO->attributesCount = count;
}

void SR_SetVertexAttribute(size_t index, size_t count, size_t stride, size_t offset)
/* Set the pointer to a vertex attribute in the vertex buffer of the currently bound vao. */
{
    if (_pCurrVAO == NULL || index >= _pCurrVAO->attributesCount)
	return;

    _pCurrVAO->attributes[index] = (SR_VertexAttribute) {
        .count = count,
	.stride=stride,
	.offset=offset
    };
}

void SR_BindShader(enum SR_SHADER_TYPE type, SR_Shader shader)
/* Set the callback to the shader function of the given type. */
{
    switch(type) {
    case SR_VERTEX_SHADER:
	_cbVertexShader = shader;
	break;
    case SR_FRAGMENT_SHADER:
	_cbFragmentShader = shader;
	break;
    default:
	break;
    }
}

void SR_DrawArray(enum SR_PRIMITIVE_TYPE type, size_t count, size_t startindex)
/* Initiate the pipeline. */
{
    if (_pCurrVAO == NULL ||
	_pCurrVAO->vertexBuffer == NULL ||
	_pCurrVAO->indexBuffer == NULL)
	return;

    double *vertexdata = malloc(_pCurrVAO->vertexBufferSize);
    memcpy(vertexdata, _pCurrVAO->vertexBuffer, _pCurrVAO->vertexBufferSize);

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
	    index = _pCurrVAO->indexBuffer[k];
	    pVertex = &vertexdata[index*nCompPerVert];
	    perspectiveDivide(pVertex);
	}
	
	// Viewport transform
	for (size_t k = i; k < i+nVertPerPrim; k++) {
	    index = _pCurrVAO->indexBuffer[k];
	    pVertex = &vertexdata[index*nCompPerVert];
	    viewportTransform(pVertex, &windowCord[(k-i)*2]);
	}
	
	SR_Texel texel = (SR_Texel)(SR_RGBA8){.r=255,.g=100,.b = 100,.a=255};
	SR_WriteTriangle(&_framebuffer.colorBuffer, windowCord, &texel);
    }

    free(vertexdata);
}
