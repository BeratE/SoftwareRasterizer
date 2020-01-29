#include "pipeline.h"
#include "rasterizer.h"
#include <matrix.h>
#include <string.h>
#include <math.h>

/* ~Global State~ */

// Currently bound framebuffer object
static SR_FrameBuffer _framebuffer;

// Linked list of Vertex Array objects
static size_t _nextListIndex;
static struct listVAO {
    size_t index;
    SR_VertexArray* pArrayObject;
    struct listVAO* pNext;
} *_listHead;
// Pointer to currently bound VAO
static SR_VertexArray *_pCurrVAO;

// Currently bound shader pipeline
static SR_Pipeline _pipeline;

/* ~/Global State/~ */

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
/* Destroy the vertex shader indexed by the given handle. */
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
	.stride = stride,
	.offset = offset
    };
}

void SR_BindPipeline(SR_Pipeline *pipeline)
/* Set the callback to the shader function of the given type. */
{
    _pipeline.vertexShader = pipeline->vertexShader;
    _pipeline.fragmentShader = pipeline->fragmentShader;
}

void SR_DrawArray(enum SR_PRIMITIVE_TYPE prim_type, size_t count, size_t startindex)
/* Initiate the pipeline. */
{
    if (_pCurrVAO == NULL ||
	_pCurrVAO->indexBuffer == NULL ||
	_pCurrVAO->vertexBuffer == NULL ||
	_pipeline.vertexShader == NULL)
	return;

    const size_t indexBufferCount = _pCurrVAO->indexBufferSize / sizeof(*_pCurrVAO->indexBuffer);
    if (startindex + count > indexBufferCount) // Index out of bounds
	return;

    // Determine appropiate write function for primitive type
    SR_Write cbWrite = NULL;
    switch(prim_type) {
    case SR_POINTS:    cbWrite = &SR_WritePixel; break;
    case SR_LINES:     cbWrite = &SR_WriteLine; break;
    case SR_TRIANGLES: cbWrite = &SR_WriteTriangle; break;
    default:
	break;
    }
    if (cbWrite == NULL)
	return;

    const size_t VERT_PER_PRIM = prim_type;
    const size_t WIDTH  = _framebuffer.colorBuffer.width;
    const size_t HEIGHT = _framebuffer.colorBuffer.height;

    int uvWindow[VERT_PER_PRIM * 2]; // Per Patch
    SR_Vec4f vPositions[VERT_PER_PRIM]; // Per Patch
    SR_Vecf attribs[_pCurrVAO->attributesCount]; //Per Vertex

    // Vertex iteration
    for (size_t i = 0; i < count; i++) {
	const size_t primVertCount = i % VERT_PER_PRIM;
	const size_t elementIndex = _pCurrVAO->indexBuffer[startindex + i];

	// Collect Vertex Attributes
	for (size_t ai = 0; ai < _pCurrVAO->attributesCount; ai++) {
	    const SR_VertexAttribute va =_pCurrVAO->attributes[ai];
	    // Pointer to vertex attribute location
	    const unsigned char *pVertexData = ((unsigned char*)_pCurrVAO->vertexBuffer)
		+ va.offset
		+ (elementIndex * va.stride);
	    
	    switch (va.count) {
	    case 4: attribs[ai].vec4f.w = *(((double*)pVertexData)+3); /* FALLTHRU */
	    case 3: attribs[ai].vec3f.z = *(((double*)pVertexData)+2); /* FALLTHRU */
	    case 2: attribs[ai].vec2f.y = *(((double*)pVertexData)+1); /* FALLTHRU */
	    case 1: attribs[ai].vec1f.x = *(((double*)pVertexData)+0);
		break;
	    default:
		break;
	    }
	}

	// Vertex assembly
	SR_Vec4f vPos = (SR_Vec4f){0, 0, 0, 0};
	(*_pipeline.vertexShader)(_pCurrVAO->attributesCount, attribs, &vPos);

	// Perspective divide
	if (vPos.w != 0) {
	    vPos.x /= vPos.w;
	    vPos.y /= vPos.w;
	    vPos.z /= vPos.w;
	    vPos.w = 1.0;
	}

	// Collect vertices for primitive before rasterization
	vPositions[primVertCount] = vPos;
	if (primVertCount == (VERT_PER_PRIM - 1)) {
	    // Viewport transform;
	    for(size_t k = 0; k < VERT_PER_PRIM; k++) {
		uvWindow[2*k+0] = vPositions[k].x * WIDTH/2 + WIDTH/2;
		uvWindow[2*k+1] = vPositions[k].y * HEIGHT/2 + HEIGHT/2;
	    }

	    // Rasterization
	    SR_Texel texel = (SR_Texel)(SR_RGBA8){.r=255,.g=100,.b = 100,.a=255};
	    (*cbWrite)(&_framebuffer.colorBuffer, uvWindow, &texel);
	}
    }
}
