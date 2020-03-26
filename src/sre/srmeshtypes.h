/* SR Mesh type definitions */
#ifndef SRMESHTYPES_H
#define SRMESHTYPES_H

#include "sretypes.h"

// Model loading
typedef struct {
    int primitiveType;
    union {
	size_t *indices; 
	struct {
	    size_t vertexIndex;
	    size_t texUVIndex;
	    size_t normalIndex;
	} *indexTuple; 
    };
} SRM_Face;
#define SRM_NULL_FACE (SRM_Face){.primitiveType = 0, .indices = NULL}

typedef struct {
    char *name;
    size_t nFaces;
    size_t nTextureUVs;
    size_t nVertices;
    size_t nNormals;
    // Vertex Data
    union {
	float *vertices;
	SR_Vec3f *vertexTuples;
    };
    union {
	float *textureUVs;
	SR_Vec2f *uvTuples;
    };
    union {
	float *normals;
	SR_Vec3f *normalTuples;
    };
    // Face Data
    SRM_Face *faces;
} SRM_Mesh;
#define SRM_NULL_MESH (SRM_Mesh) {      \
      .name = NULL,			\
      .vertices = NULL,			\
      .textureUVs = NULL,		\
      .normals = NULL,			\
      .faces = NULL,			\
      .nFaces = 0,			\
      .nNormals = 0,			\
      .nVertices = 0,			\
      .nTextureUVs = 0}			\


#endif // SRMESHTYPES_H
