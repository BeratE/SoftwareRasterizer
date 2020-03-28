/* Model loading and mesh generation. */
#ifndef SRMESH_H
#define SRMESH_H

#include "../sretypes.h"

// Common defintions
#define EOS '\0'
#define NEWLINE '\n'
#define WHITESPACE ' '

// Wavefront object format
#define OBJ_COMMENT         '#'
#define OBJ_NAME            'o'
#define OBJ_VERTEX          'v'
#define OBJ_VERTEX_TEXTURE  't'
#define OBJ_VERTEX_NORMAL   'n'
#define OBJ_FACE            'f'
#define OBJ_DELIMITER       '/'


typedef struct {
  /* A Face is a collection of (1-based) indices into the vertex data of
   * the mesh. An index of 0 means that there is no corresponding value.
   * The face consists of three vertex tuples of (vertex, texture, normal)
   * indices each. */
    size_t indices[9]; 
} SRM_Face;

typedef struct {
    SR_Boolean isInit;
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
    .isInit = FALSE,		        \
    .name = NULL,			\
    .vertices = NULL,			\
    .textureUVs = NULL,		        \
    .normals = NULL,			\
    .faces = NULL,			\
    .nFaces = 0,			\
    .nNormals = 0,			\
    .nVertices = 0,			\
    .nTextureUVs = 0}




SR_Boolean SRM_LoadMesh(SRM_Mesh *mesh, const char *filepath);
int SRM_IndexedVertexData(SRM_Mesh *mesh, float *outVertexData,
			  size_t *outIndices, size_t *outVertexCount);
void SRM_DeleteMesh(SRM_Mesh *mesh);
void SRM_PrintMesh(SRM_Mesh *mesh);

#endif // SRMESH_H
