#include "srmesh.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void collectLineFloat(char *lp, float **array, size_t *num)
{
    while(*(++lp) == ' ');
    ;
    while (*lp != '\0') {
	(*array) = realloc((*array), ((*num)+1)*sizeof(float));
	(*array)[*num] = atof(lp);
	(*num)++;
	while(*(++lp) != ' ' && *lp != '\0');
	;
    }   
}


static void copyLineString(char *lp, char** str)
{
    while(*(++lp) == ' ');
    ;
    (*str) = malloc(strlen(lp) + 1);
    strcpy((*str), lp);    
}

int SRM_LoadMesh(SRM_Mesh *mesh, const char *filepath)
/* Return a mesh object created from the contents of the given wavefront file. */
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
	perror("Error opening file");
	return -1;
    }

    mesh->faces = NULL;
    mesh->normals = NULL;
    mesh->vertices = NULL;
    mesh->textureUVs = NULL;
    mesh->nFaces = 0;
    mesh->nNormals = 0;
    mesh->nVertices = 0;
    mesh->nTextureUVs = 0;

    char line[255];
    while (fgets(line, 255, file) != NULL) {
	char *lp = line;

	// skip empty lines and comments
	while (*lp == ' ' && lp++);
	if (*lp == '\0' || *lp == '#')
	    continue;

	if (lp[0] == 'o') {
	    copyLineString(lp, &mesh->name);
	}

	// Vertex Data
	else if (lp[0] == 'v') {
	    lp++;
	    if (lp[0] == ' ') {
		collectLineFloat(lp, &mesh->vertices, &mesh->nVertices);
	    } else if (lp[0] == 't') {
		collectLineFloat(lp, &mesh->textureUVs, &mesh->nTextureUVs);
	    } else if (lp[0] == 'n') {
		collectLineFloat(lp, &mesh->normals, &mesh->nNormals);
	    }
	}

	// Face Data
	else if (lp[0] == 'f') {
	    while(*(++lp) == ' ');
	    ;
	    mesh->faces = realloc(mesh->faces, (mesh->nFaces+1)*sizeof(SRM_Face));
	    SRM_Face *face = &mesh->faces[mesh->nFaces];
	    face->primitiveType = 0;
	    face->indices = NULL;
	    
	    while(*lp != '\0') {
		face->primitiveType++;
		face->indices = realloc(face->indices,
					3*face->primitiveType*sizeof(size_t));

		for (int i = 0; i < 3; i++) {
		    face->indices[3*(face->primitiveType-1)+i] = atoi(lp);

		    while(*lp != '/' && *lp != ' ' && *lp != '\0' && lp++);
		    ;
		    if (*lp == '\0')
			break;
		    else if (*lp == ' ') {
			lp++;
			break;
		    }
		    lp++;
		}
	    }

	    mesh->nFaces++;
	}
    }

    fclose(file);

    return 0;
}


int SRM_PrintMesh(SRM_Mesh *mesh)
/* Print the contents of the given mesh in human readable form. */
{
    printf("Name: %s", mesh->name);

    printf("\nVertices:\n");
    for (unsigned int i = 0; i < mesh->nVertices; i++) {
	printf("%f, ", mesh->vertices[i]);
	if (((i + 1) % 3) == 0)
	    printf("\n");
    }
    
    printf("\nTextureUVs:\n");
    for (unsigned int i = 0; i < mesh->nTextureUVs; i++) {
	printf("%f, ", mesh->textureUVs[i]);
	if (((i + 1) % 2) == 0)
	    printf("\n");
    }
    
    printf("\nNormals:\n");
    for (unsigned int i = 0; i < mesh->nNormals; i++) {
	printf("%f, ", mesh->normals[i]);
	if (((i + 1) % 3) == 0)
	    printf("\n");
    }
    
    printf("\nFace Indices:\n");
    for (unsigned int i = 0; i < mesh->nFaces; i++) {
	for (int j = 0; j < mesh->faces[i].primitiveType; j++) {
	    for(int k = 0; k < 3; k++) {
		printf("%ld/", mesh->faces[i].indices[(3*j)+k]);
	    }
	    printf(" ");
	}
	printf("\n");
    }

    return 0;
}

int SRM_IndexedMeshVertexData(SRM_Mesh *mesh, float *outVertexData,
			      size_t *outIndices, size_t *outVertexCount)
{
    const size_t VERTS_PER_FACE = 3;
    const size_t MAX_VERT_COUNT = mesh->nFaces * VERTS_PER_FACE;

    size_t indices[MAX_VERT_COUNT];
    float vData[MAX_VERT_COUNT * 8];

    memset(indices, 0, MAX_VERT_COUNT*sizeof(size_t));

    size_t nVerts = 0; // Number of vertices
    for (size_t i = 0; i < MAX_VERT_COUNT; i++) {
	const size_t f_i = i / VERTS_PER_FACE; // face index [0, .., nFaces]
	const size_t v_i = i % VERTS_PER_FACE; // local vertex index [0, 1, 2]
	const size_t *vert_i = &mesh->faces[f_i].indices[mesh->faces[f_i].primitiveType * v_i];

	// Check if this vertex was visited
	size_t visited = 0;
	for (size_t j = 0; j < i; j++) {
	    if (indices[j] == 0)
		break;

	    const size_t f_j = j / VERTS_PER_FACE;
	    const size_t v_j = j % VERTS_PER_FACE;
	    const size_t *vert_j = &mesh->faces[f_j].indices[mesh->faces[f_j].primitiveType * v_j];

	    if (vert_i[0] == vert_j[0] &&
		vert_i[1] == vert_j[1] &&
		vert_i[2] == vert_j[2]) {
		visited = indices[j];
		break;
	    }
	}

	if (!visited) {
	    if (outVertexData != NULL) {
		memcpy(&vData[nVerts * 8 + 0],
		       &mesh->vertices[(vert_i[0] - 1) * 3],
		       3 * sizeof(float));
		memcpy(&vData[nVerts * 8 + 3],
		       &mesh->textureUVs[(vert_i[1] - 1) * 2],
		       2 * sizeof(float));
		memcpy(&vData[nVerts * 8 + 5],
		       &mesh->normals[(vert_i[2] - 1) * 3],
		       3 * sizeof(float));
	    }
	    indices[i] = ++nVerts;
	} else {
	    indices[i] = visited;
	}
    }
    
    if (outVertexCount != NULL)
	*outVertexCount = nVerts;
    if (outIndices != NULL)
	memcpy(outIndices, indices, MAX_VERT_COUNT*sizeof(size_t));
    if (outVertexData != NULL)
	memcpy(outVertexData, vData, nVerts*8*sizeof(float));

    return 0;
}
