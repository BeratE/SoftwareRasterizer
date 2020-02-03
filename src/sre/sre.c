#include "sre.h"
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

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
SR_Pipeline _pipeline;

/* ~/Global State/~ */


/* Static functions */

static inline void collectVertexAttribs(SR_Vecf *attribs, size_t elementIndex)
{
    for (size_t ai = 0; ai < _pCurrVAO->attributesCount; ai++) {
	const SR_VertexAttribute va =_pCurrVAO->attributes[ai];
	// Pointer to vertex attribute location
	const uint8_t *pVertexData = ((unsigned char*)_pCurrVAO->vertexBuffer)
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
}

static inline void perspectiveDivide(SR_Vec4f *p)
{
    if (p->w != 0) {
	p->x /= p->w;
	p->y /= p->w;
    }
}

/* ~/Static functions/~ */


/* 
 * Setup, Shutdown
 */

void SR_Init()
/* Initialize the software rasterization engine. */
{
    SR_SetViewPort(0, 0);
    
    _pipeline = (SR_Pipeline){
	.vertexShader = NULL, .fragmentShader = NULL,
	.currVertexStageOutput = NULL, .pVertexStageOutput = NULL,
	.vertexStageOutputCount = 0};
    
    _listHead = NULL;
    _pCurrVAO = NULL;
    _nextListIndex = 0;
}

void SR_Shutdown()
/* Clean up resources. */
{
    // Free Framebuffer
    SR_TexBufferFree(&_framebuffer.color);
    SR_TexBufferFree(&_framebuffer.depth);

    if (_pipeline.currVertexStageOutput != NULL)
	free(_pipeline.currVertexStageOutput);

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


/* 
 * Framebuffer 
 */

SR_FrameBuffer SR_GetFrameBuffer()
/* Temporary solution. */
{
    return _framebuffer;
}

void SR_SetViewPort(int w, int h)
/* Set global viewport state parameters. */
{
    SR_TexBufferFree(&_framebuffer.color);
    SR_TexBufferFree(&_framebuffer.depth);
    _framebuffer.color = SR_TexBufferCreate(w, h, SR_TEX_FORMAT_RGBA8);
    _framebuffer.depth = SR_TexBufferCreate(w, h, SR_TEX_FORMAT_32F);
}

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask)
/* Clear the target buffer with zero-values. */
{
    if (buffermask & SR_COLOR_BUFFER_BIT) {
	int clearVal = 0;
	SR_TexBufferClear(&_framebuffer.color, &clearVal);
    }
    if (buffermask & SR_DEPTH_BUFFER_BIT) {
	float clearVal = FLT_MAX;
	SR_TexBufferClear(&_framebuffer.depth, &clearVal);
    }
}


/* 
 * Vertex Array Objects
 */

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

    void** buffer = NULL;
    switch (target) {
    case SR_VERTEX_BUFFER:
	_pCurrVAO->vertexBufferSize = size;
	buffer = (void**)&_pCurrVAO->vertexBuffer;
	break;
    case SR_INDEX_BUFFER:
	_pCurrVAO->indexBufferSize = size;
	buffer = (void**)&_pCurrVAO->indexBuffer;
	break;
    }
    
    if (buffer != NULL) {
	(*buffer) = malloc(size);
	memcpy((*buffer), data, size);
    }
}


/*
 * Vertex Attributes
 */

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


/* 
 * Pipeline
 */

void SR_BindShader(enum SR_SHADER_TYPE shader_type, SR_ShaderCB shader)
/* Set the callback to the shader function of the given type. */
{
    switch (shader_type) {
    case SR_VERTEX_SHADER:
	_pipeline.vertexShader = shader;
	break;
    case SR_FRAGMENT_SHADER:
	_pipeline.fragmentShader = shader;
	break;
    };
}

void SR_SetVertexStageOutputCount(size_t count)
/* Set the number of output values of the vertex shader stage. */
{
    if (_pipeline.currVertexStageOutput != NULL)
	free(_pipeline.currVertexStageOutput);

    _pipeline.vertexStageOutputCount = count;
    _pipeline.currVertexStageOutput = malloc(count * sizeof(SR_Vecf));
}

void SR_SetVertexStageOutput(size_t index, SR_Vecf* value)
/* Set the value of the vertex stage output at the given index. */
{
    if (_pipeline.currVertexStageOutput == NULL)
	return;
    memcpy(&_pipeline.currVertexStageOutput[index], value, sizeof(*value));
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
	//case SR_POINTS:    cbWrite = &SR_WritePixel; break;
    case SR_LINES:     cbWrite = &SR_WriteLine; break;
    case SR_TRIANGLES: cbWrite = &SR_WriteTriangle; break;
    default:
	break;
    }
    if (cbWrite == NULL)
	return;

    const size_t VERT_PER_PRIM = prim_type;
    const size_t VERT_OUTPUT_N = _pipeline.vertexStageOutputCount;
    const size_t WIDTH  = _framebuffer.color.width;
    const size_t HEIGHT = _framebuffer.color.height;

    // Per Patch
    SR_VecScreen patchScreenCoords[VERT_PER_PRIM];
    SR_Vec4f patchVertPos[VERT_PER_PRIM];     
    SR_Vecf patchVertStageOutput[VERT_PER_PRIM][VERT_OUTPUT_N];
    _pipeline.pVertexStageOutput = (SR_Vecf*)patchVertStageOutput;
    //Per Vertex
    SR_Vecf vertAttribs[_pCurrVAO->attributesCount]; 

    // Vertex iteration
    for (size_t i = 0; i < count; i++) {
	const size_t patchVertNum = i % VERT_PER_PRIM;
	const size_t elementIndex = _pCurrVAO->indexBuffer[startindex + i];

	// Collect vertex attributes from buffer
	collectVertexAttribs(vertAttribs, elementIndex);

	// Vertex assembly
	SR_Vec4f vPos = (SR_Vec4f){0, 0, 0, 0};
	(*_pipeline.vertexShader)(_pCurrVAO->attributesCount, vertAttribs, &vPos);

	// Collect vertex stage output
	memcpy(&patchVertStageOutput[patchVertNum][0],
	       _pipeline.currVertexStageOutput, sizeof(SR_Vecf) * VERT_OUTPUT_N);

	// Perspective divide
	perspectiveDivide(&vPos);
	
	// Collect patch vertices
	patchVertPos[patchVertNum] = vPos;
	if (patchVertNum == (VERT_PER_PRIM - 1)) {
	    // Viewport transform;
	    for(size_t k = 0; k < VERT_PER_PRIM; k++) {
		patchScreenCoords[k].x = patchVertPos[k].x * WIDTH/2 + WIDTH/2;
		patchScreenCoords[k].y = patchVertPos[k].y * HEIGHT/2 + HEIGHT/2;
		patchScreenCoords[k].z = patchVertPos[k].z;
	    }

	    // Rasterization
	    (*cbWrite)(&_framebuffer, patchScreenCoords, &_pipeline);
	}
    }

    // Cleanup
    _pipeline.pVertexStageOutput = NULL;
}
