#ifndef COLORBUFFER_H
#define COLORBUFFER_H
/* Data Types and Functions for operating on color buffers. 
 * Memory is stored in an array of size width*height*format_size bytes. 
 * format_size determines the number of components per color entry. */

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char *array;
    unsigned char format_size;     // Number of Components
} ColorBuffer;

typedef struct{
    unsigned char b, g, r, a;
} ColorBGRA8;

ColorBuffer* CreateColorBuffer(unsigned int width, unsigned int height,
			       unsigned char format_size);

void ClearColorBuffer(ColorBuffer *buffer, unsigned char value);

#endif // COLORBUFFER_H
