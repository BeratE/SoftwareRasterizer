/* Model loading and mesh generation. */
#ifndef SRMESH_H
#define SRMESH_H

#include "srmeshtypes.h"

int SRM_LoadMesh(SRM_Mesh *mesh, const char *filepath);
int SRM_PrintMesh(SRM_Mesh *mesh);

int SRM_IndexedMeshVertexData(SRM_Mesh *mesh, float *outVertexData,
			      size_t *outIndices, size_t *outVertexCount);

#endif // SRMESH_H
