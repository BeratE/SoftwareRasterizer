/* SR Mesh type definitions */
#ifndef SRMESHTYPES_H
#define SRMESHTYPES_H

#include <stddef.h>

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

typedef struct {
    char *name;
    size_t nFaces;
    size_t nTextureUVs;
    size_t nVertices;
    size_t nNormals;
    // Vertex Data
    union {
	float *vertices;
	struct {
	    float x, y, z;
	} *vertexTuples;
    };
    union {
	float *textureUVs;
	struct {
	    float u, v;
	} *uvTuples;
    };
    union {
	float *normals;
	struct {
	    float x, y, z;
	} *normalTuples;
    };
    // Face Data
    SRM_Face *faces;
} SRM_Mesh;

#endif // SRMESHTYPES_H
