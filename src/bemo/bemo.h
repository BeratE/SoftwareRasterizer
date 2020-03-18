#ifndef BEMO_MESH_H
#define BEMO_MESH_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t nVertices, nNormals, nTextureUV, nFaces;
    uint8_t hasNormals, hasTextureUV;
    char *name;
    size_t *faces;
    double *vertices, *normals, *textureUV;
} BEMO_Mesh;

int BEMO_LoadMesh(BEMO_Mesh *scene, const char* filepath);

#endif // BE_MESH_H_H
