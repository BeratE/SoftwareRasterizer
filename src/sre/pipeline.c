#include "pipeline.h"
#include "string.h"

/* ~Global State~ */
// Generic collection of allocated bufferdata elements
static void** _bufferdata = NULL;
static size_t _buffercount;     // Number of buffer elements in the bufferdata array
static size_t _bufferhandle;    // Current handle, index to bufferdata element
static size_t _boundbuffers[1]; // Currently bound buffers

static SR_ViewPort _viewPort;
/* ~Global State~ */

/* Static functions */
static void allocBufferData(size_t count)
/* Allocate space for the bufferdata element array. */
{
    _buffercount += count;
    _bufferdata = realloc(_bufferdata, _buffercount * sizeof(*_bufferdata));
}

/* Static functions */

void SR_Init()
/* Initialize the software rasterization engine. */
{
    for(size_t i = 0; i < (sizeof(_boundbuffers)/sizeof(_boundbuffers[0])); i++) 
	_boundbuffers[i] = 0;

    _bufferhandle = 0;
    
    if (_bufferdata != NULL) {
	free(_bufferdata);
    }
    _buffercount = 1;
    _bufferdata = malloc(sizeof *_bufferdata);
    
    _viewPort = (SR_ViewPort){0, 0, 0, 0};
}

void SR_GenBuffers(int* handles, size_t count)
{
    if (_bufferhandle + count > _buffercount)
	allocBufferData(count-_bufferhandle);
	
    for (size_t i = 0; i < 0; i++) {
	handles[i] = _bufferhandle;
	_bufferhandle++;
    }
}

void SR_SetBufferData(int handle, void *data, size_t arrsize)
/* Copy the data source to the bufferdata of the given object handle. */
{
    if (_bufferdata[handle] != NULL)
	free(_bufferdata[handle]);

    _bufferdata[handle] = malloc(arrsize);
    memcpy(_bufferdata[handle], data, arrsize);
}

void SR_BindBuffer(int buffertype, int handle)
/* Binds the given handle as the current buffer object of given type. */
{
    _boundbuffers[buffertype] = handle;
}

void SR_SetViewPort(int x, int y, int w, int h)
/* Set global viewport state parameters.  */
{
    _viewPort.x = x;
    _viewPort.y = y;
    _viewPort.w = w;
    _viewPort.h = h;
}

