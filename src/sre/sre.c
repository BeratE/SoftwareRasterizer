#include "sre.h"
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

/* ~Global State~ */
SR_Pipeline __sr_pipeline; // Currently bound shader pipeline

/* ~/Global State/~ */

/* ~Local State ~*/
static SR_FrameBuffer __sr_framebuffer; // Currently bound framebuffer

// Linked list of Vertex Array objects
static size_t __sr_nextListIndex;
static struct listVAO {
    size_t index;
    SR_VertexArray* pArrayObject;
    struct listVAO* pNext;
} *__sr_listHead;

static SR_VertexArray *__sr_pCurrVAO; // Pointer to currently bound VAO

/* ~Local State ~*/


/* Static functions */

static inline void collectVertexAttribs(SR_Vecf *attribs, size_t elementIndex)
    /* Collect Vertex Attributes from the currently bound buffer,
     * starting at the given element index. */
{
    for (size_t ai = 0; ai < __sr_pCurrVAO->attributesCount; ai++) {
	const SR_VertexAttribute va = __sr_pCurrVAO->attributes[ai];
	// Pointer to vertex attribute location
	const uint8_t *pVertexData = ((uint8_t *)__sr_pCurrVAO->vertexBuffer)
	    + va.offset + (elementIndex * va.stride);

	switch (va.count) {
	case 4: attribs[ai].vec4f.w = *(((float*)pVertexData)+3); /* FALLTHRU */
	case 3: attribs[ai].vec3f.z = *(((float*)pVertexData)+2); /* FALLTHRU */
	case 2: attribs[ai].vec2f.y = *(((float*)pVertexData)+1); /* FALLTHRU */
	case 1: attribs[ai].vec1f.x = *(((float*)pVertexData)+0);
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

    __sr_pipeline = (SR_Pipeline){.vertexShader = NULL,
                                  .fragmentShader = NULL,
                                  .currVertexStageOutput = NULL,
                                  .pVertexStageOutput = NULL,
                                  .vertexStageOutputCount = 0};

    __sr_listHead = NULL;
    __sr_pCurrVAO = NULL;
    __sr_nextListIndex = 0;
}

void SR_Shutdown()
    /* Clean up resources. */
{
    // Free Framebuffer
    SR_TexBufferFree(&__sr_framebuffer.color);
    SR_TexBufferFree(&__sr_framebuffer.depth);

    if (__sr_pipeline.currVertexStageOutput != NULL)
      free(__sr_pipeline.currVertexStageOutput);

    // Free Vertex Array Data
    struct listVAO *listp = __sr_listHead;
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
  return __sr_framebuffer;
}

void SR_SetViewPort(int w, int h)
    /* Set global viewport state parameters. */
{
  SR_TexBufferFree(&__sr_framebuffer.color);
  SR_TexBufferFree(&__sr_framebuffer.depth);
  __sr_framebuffer.color = SR_TexBufferCreate(w, h, SR_TEX_FORMAT_RGBA8);
  __sr_framebuffer.depth = SR_TexBufferCreate(w, h, SR_TEX_FORMAT_F32);
}

void SR_Clear(enum SR_RENDER_TARGET_BIT buffermask)
    /* Clear the target buffer with zero-values. */
{
    if (buffermask & SR_RTB_COLOR_BUFFER_BIT) {
	int clearVal = 0;
        SR_TexBufferClear(&__sr_framebuffer.color, &clearVal);
    }
    if (buffermask & SR_RTB_DEPTH_BUFFER_BIT) {
	float clearVal = FLT_MAX;
        SR_TexBufferClear(&__sr_framebuffer.depth, &clearVal);
    }
}


/* 
 * Vertex Array Objects
 */

size_t SR_GenVertexArray()
    /* Generate a new vertex array object and return the object handle. */
{
    size_t returnindex;
    if (__sr_listHead == NULL) {
      __sr_listHead = malloc(sizeof(struct listVAO));
      __sr_listHead->index = ++__sr_nextListIndex;
      __sr_listHead->pNext = NULL;

      __sr_listHead->pArrayObject = malloc(sizeof(SR_VertexArray));
      __sr_listHead->pArrayObject->indexBuffer = NULL;
      __sr_listHead->pArrayObject->vertexBuffer = NULL;
      __sr_listHead->pArrayObject->attributes = NULL;
      __sr_listHead->pArrayObject->attributesCount = 0;

      returnindex = __sr_listHead->index;
    } else {
      struct listVAO *listp = __sr_listHead;
      while ((listp->pNext != NULL) && (listp = listp->pNext))
        ;

      listp->pNext = malloc(sizeof(struct listVAO));
      listp->pNext->index = ++__sr_nextListIndex;
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
    /* Destroy the vertex shader indexed by the given handle.*/
{
    if (__sr_listHead == NULL || handle > __sr_nextListIndex)
	return;

    struct listVAO *listp = __sr_listHead;
    struct listVAO *nextp = __sr_listHead->pNext;
    if (__sr_listHead->index == handle) {
	__sr_listHead = nextp;
	free(listp->pArrayObject->indexBuffer);
	free(listp->pArrayObject->vertexBuffer);
	free(listp->pArrayObject->attributes);
	free(listp->pArrayObject);
	free(listp);
    } else {
	while (nextp != NULL && nextp->index != handle) {
	    listp = nextp;
	    nextp = nextp->pNext;
	}
    }
}

void SR_BindVertexArray(size_t handle)
    /* Set the vertex array object identified by handle
     * as the currently bound vao. */
{
    struct listVAO *listp = __sr_listHead;
    while ((listp != NULL) && (listp->index != handle) && (listp = listp->pNext));
    ;
    if (listp != NULL)
	__sr_pCurrVAO = listp->pArrayObject;
    else
	__sr_pCurrVAO = NULL;
}

void SR_SetBufferData(enum SR_BUFFER_TYPE target, void *data, size_t size)
    /* Set the vertex buffer data of the currently bound vao. */
{
    if (__sr_pCurrVAO == NULL)
	return;

    void **buffer = NULL;
    switch (target) {
    case SR_BT_VERTEX_BUFFER:
	__sr_pCurrVAO->vertexBufferSize = size;
	buffer = (void **)&__sr_pCurrVAO->vertexBuffer;
	break;
    case SR_BT_INDEX_BUFFER:
	__sr_pCurrVAO->indexBufferSize = size;
	buffer = (void **)&__sr_pCurrVAO->indexBuffer;
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
    if (__sr_pCurrVAO == NULL)
	return;
    if (__sr_pCurrVAO->attributes != NULL)
	free(__sr_pCurrVAO->attributes);
    
    __sr_pCurrVAO->attributes = malloc(sizeof(SR_VertexAttribute) * count);
    __sr_pCurrVAO->attributesCount = count;
}

void SR_SetVertexAttribute(size_t index, size_t count,
			   size_t stride, size_t offset)
    /* Set the pointer to a vertex attribute in the vertex buffer of the
    currently bound vao. */
{
    if (__sr_pCurrVAO == NULL || index >= __sr_pCurrVAO->attributesCount)
	return;

    __sr_pCurrVAO->attributes[index] =
	(SR_VertexAttribute){.count = count, .stride = stride, .offset = offset};
}


/* 
 * Pipeline
 */

void SR_BindShader(enum SR_SHADER_TYPE shader_type, SR_ShaderCB shader)
    /* Set the callback to the shader function of the given type. */
{
    switch (shader_type) {
    case SR_ST_VERTEX_SHADER:
	__sr_pipeline.vertexShader = shader;
	break;
    case SR_ST_FRAGMENT_SHADER:
	__sr_pipeline.fragmentShader = shader;
	break;
    };
}

void SR_SetVertexStageOutputCount(size_t count)
    /* Set the number of output values of the vertex shader stage. */
{
    if (__sr_pipeline.currVertexStageOutput != NULL)
	free(__sr_pipeline.currVertexStageOutput);

    __sr_pipeline.vertexStageOutputCount = count;
    __sr_pipeline.currVertexStageOutput = malloc(count * sizeof(SR_Vecf));
}

void SR_SetVertexStageOutput(size_t index, SR_Vecf *value)
    /* Set the value of the vertex stage output at the given index. */
{
    if (__sr_pipeline.currVertexStageOutput == NULL)
	return;
    
    memcpy(&__sr_pipeline.currVertexStageOutput[index], value, sizeof(*value));
}

void SR_DrawArray(enum SR_PRIMITIVE_TYPE prim_type,
		  size_t count, size_t startindex)
    /* Initiate the pipeline. */
{
    if (__sr_pCurrVAO == NULL || __sr_pCurrVAO->indexBuffer == NULL ||
	__sr_pCurrVAO->vertexBuffer == NULL || __sr_pipeline.vertexShader == NULL)
	return;

    const size_t indexBufferCount =
	__sr_pCurrVAO->indexBufferSize / sizeof(*__sr_pCurrVAO->indexBuffer);
    if (startindex + count > indexBufferCount) // Index out of bounds
	return;

    // Determine appropiate write function for primitive type
    SR_Write cbWrite = NULL;
    switch (prim_type) {
	// case SR_POINTS:    cbWrite = &SR_WritePixel; break;
    case SR_PT_LINES:
	cbWrite = &SR_WriteLine;
	break;
    case SR_PT_TRIANGLES:
	cbWrite = &SR_WriteTriangle;
	break;
    default:
	break;
    }
    if (cbWrite == NULL)
	return;

    const size_t VERT_PER_PRIM = prim_type;
    const size_t VERT_OUTPUT_N = __sr_pipeline.vertexStageOutputCount;
    const size_t WIDTH = __sr_framebuffer.color.width;
    const size_t HEIGHT = __sr_framebuffer.color.height;

    // Per Patch
    SR_VecScreen patchScreenCoords[VERT_PER_PRIM];
    SR_Vec4f patchVertPos[VERT_PER_PRIM];     
    SR_Vecf patchVertStageOutput[VERT_PER_PRIM][VERT_OUTPUT_N];
    __sr_pipeline.pVertexStageOutput = (SR_Vecf *)patchVertStageOutput;
    //Per Vertex
    SR_Vecf vertAttribs[__sr_pCurrVAO->attributesCount];

    // Vertex iteration
    for (size_t i = 0; i < count; i++) {
	const size_t patchVertNum = i % VERT_PER_PRIM;
        const size_t elementIndex = __sr_pCurrVAO->indexBuffer[startindex + i];

        // Collect vertex attributes from buffer
	collectVertexAttribs(vertAttribs, elementIndex);

	// Vertex assembly
	SR_Vec4f vPos = (SR_Vec4f){0, 0, 0, 0};
        (*__sr_pipeline.vertexShader)(__sr_pCurrVAO->attributesCount,
                                      vertAttribs, &vPos);

        // Collect vertex stage output
        memcpy(&patchVertStageOutput[patchVertNum][0],
               __sr_pipeline.currVertexStageOutput,
               sizeof(SR_Vecf) * VERT_OUTPUT_N);

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
            (*cbWrite)(&__sr_framebuffer, patchScreenCoords, &__sr_pipeline);
        }
    }

    // Cleanup
    __sr_pipeline.pVertexStageOutput = NULL;
}
