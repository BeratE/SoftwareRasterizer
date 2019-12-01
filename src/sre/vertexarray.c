#include "vertexarray.h"
#include <string.h>

struct vaoList {
    size_t index;
    SR_VAO* array;
    struct vaoList* next;
};

static struct vaoList *_vaoListHead;
static SR_VAO *_vaop;
static size_t _nextVaoIndex;

void SR_InitVertexArray()
{
    _vaoListHead = NULL;
    _nextVaoIndex = 1;
    _vaop = NULL;
}

void SR_ShutdownVertexArray()
{
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

