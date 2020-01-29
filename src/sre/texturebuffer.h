#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H
/* Data types and functions for operating on texture buffers. 
 * Memory is stored in an array of size width*height*fsize bytes. 
 * fsize determines the number of components per color entry (in bytes). */

#include <stdlib.h>
#include "types.h"

/* Functions on Texturebuffers */
SR_TextureBuffer SR_CreateTextureBuffer(unsigned int width, unsigned int height,
					unsigned char formatsize);

SR_TextureBuffer SR_CopyTextureBuffer(const SR_TextureBuffer* buffer);

void SR_FreeTextureBuffer(SR_TextureBuffer *buffer);

void SR_ClearTextureBuffer(SR_TextureBuffer *buffer, unsigned char value);

#endif // TEXTUREBUFFER_H
