#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    double *vertices; 
    double *normals;
    double *textureUV;
    size_t *faces;
    size_t numVertices;
    size_t numNormals;
    size_t numTextureUV;
    size_t numFaces;
    uint8_t hasNormals;
    uint8_t hasTextureUV;
} SR_OBJ_Scene;

int SR_OBJ_LoadScene(SR_OBJ_Scene *scene, const char* filepath);

#endif // OBJLOADER_H
