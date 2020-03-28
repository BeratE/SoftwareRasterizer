#include "srmesh.h"
#include "SDL_video.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static function */

#define SKIP_WHITESPACE(lp) while(*lp == ' ' && lp++)


static void pushFace(SRM_Mesh *mesh, SRM_Face *face)
{
    mesh->faces = realloc(mesh->faces, (mesh->nFaces+1)*sizeof(SRM_Face));
    memcpy(&mesh->faces[mesh->nFaces], face, 3*3*sizeof(size_t));
    mesh->nFaces++;
}

static int collectFaceVertex(char **lpp, size_t *indices) {
    char *lp = *lpp;
    for (int i = 0; i < 3; i++) {
	indices[i] = atoi(lp);
	// Skip to next delimiter
	while (*lp != OBJ_DELIMITER && // Next Attribute
	       *lp != WHITESPACE &&    // Next Vertex Tuples
	       *lp != NEWLINE &&       // End of face
	       *lp != EOS)             // End of string
	    lp++;
	if (*lp == WHITESPACE || *lp == NEWLINE) {
	    lp++;
	    break;
	}
	if (*lp == EOS)
	    break;
	lp++;
    }
    *lpp = lp;
}

static SR_Boolean lineProcessFace(char *lp, SRM_Mesh *mesh) {    
    SKIP_WHITESPACE(lp);
    
    SRM_Face face;
    int vertexCount = 0;
    // Collect vertex data up to 3 data tuples
    while(*lp != EOS && vertexCount < 3) {
      collectFaceVertex(&lp, &face.indices[3*vertexCount]);
      vertexCount++;
    }

    // There shouldnt be faces with less than 3 vertices
    if (vertexCount < 3)
	return FALSE;
    
    pushFace(mesh, &face);

    // Parse a 4th component, quad in (1,2,3,4)->(1,2,3),(1,3,4)
    if (*lp != EOS) {
    	memcpy(&face.indices[3], &face.indices[6], 3*sizeof(size_t));
    	collectFaceVertex(&lp, &face.indices[6]);
	pushFace(mesh, &face);
    }
    
    return TRUE;
}

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

static void lineCopyString(char *lp, char **str) {
    SKIP_WHITESPACE(lp);
    (*str) = malloc(strlen(lp) + 1);
    strcpy((*str), lp);
}

/* ~Static function~ */


SR_Boolean SRM_LoadMesh(SRM_Mesh *mesh, const char *filepath)
    /* Return a mesh object created from the contents of the given
     * wavefront file. */
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
	perror("Error opening file");
	return FALSE;
    }

    *mesh = SRM_NULL_MESH;
    char linebuffer[255];

    SR_Boolean status = TRUE;
    
    while (fgets(linebuffer, 255, file) != NULL) {
	// Pointer to current character in linebuffer
	char *lp = linebuffer;

	// skip empty lines and comments
	SKIP_WHITESPACE(lp);
	if (*lp == OBJ_COMMENT || *lp == NEWLINE || *lp == EOS)
	    continue;

	// Object Name
	if (*lp == OBJ_NAME) {
	    lineCopyString(lp, &mesh->name);
        }
	
	// Vertex Data
	else if (*lp == OBJ_VERTEX) {
	    lp++;
	    if (*lp == WHITESPACE) {
		collectLineFloat(lp, &mesh->vertices, &mesh->nVertices);
	    } else if (*lp == OBJ_VERTEX_TEXTURE) {
		collectLineFloat(lp, &mesh->textureUVs, &mesh->nTextureUVs);
	    } else if (*lp == OBJ_VERTEX_NORMAL) {
		collectLineFloat(lp, &mesh->normals, &mesh->nNormals);
	    }
	}

	// Face Data
	else if (*lp == OBJ_FACE) {
	    if (*(++lp) != WHITESPACE ||
		lineProcessFace(lp, mesh) == FALSE) {
		status = FALSE;
		break;
	    }
        }
    }

    fclose(file);
    mesh->isInit = status;
    return status;
}

int SRM_IndexedVertexData(SRM_Mesh *mesh, float *outVertexData,
                              size_t *outIndices, size_t *outVertexCount)
    /* Parse the data in the mesh into a indexed mesh vertex data format.
    * The vertices are stored as a continous array of
    * (vx, vy, vz, u, v, nx, ny,nz). The number of distinct vertex
    * combinations is returned in the outVertexcount. */
{
    if (!mesh->isInit)
	return FALSE;
    
    const size_t VERTS_PER_FACE = 3;
    const size_t MAX_VERT_COUNT = mesh->nFaces * VERTS_PER_FACE;

    size_t indices[MAX_VERT_COUNT];
    float vData[MAX_VERT_COUNT * 8];

    memset(indices, 0, MAX_VERT_COUNT*sizeof(size_t));

    size_t nVerts = 0; // Number of vertices
    for (size_t i = 0; i < MAX_VERT_COUNT; i++) {
	const size_t f_i = i / VERTS_PER_FACE; // face index [0, .., nFaces]
	const size_t v_i = i % VERTS_PER_FACE; // local vertex index [0, 1, 2]
	const size_t *vert_i = &mesh->faces[f_i].indices[3*v_i];

	// Check if this vertex was visited
	size_t visited = 0;
	for (size_t j = 0; j < i; j++) {
	    if (indices[j] == 0)
		break;

	    const size_t f_j = j / VERTS_PER_FACE;
	    const size_t v_j = j % VERTS_PER_FACE;
	    const size_t *vert_j = &mesh->faces[f_j].indices[3*v_j];

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
		       &mesh->vertices[(vert_i[0]-1)*3],
		       3 * sizeof(float));
		if (vert_i[1]) {
		    memcpy(&vData[nVerts*8+3], &mesh->textureUVs[(vert_i[1]-1)*2],
			   2 * sizeof(float));
		} 
		if (vert_i[2]) {
		    memcpy(&vData[nVerts *8+5], &mesh->normals[(vert_i[2]-1)*3],
			   3 * sizeof(float));
		}
	    }
	    indices[i] = ++nVerts;
	} else {
	    indices[i] = visited;
	}
    }
    
    if (outVertexCount != NULL)
	*outVertexCount = nVerts;
    if (outIndices != NULL) {
	// Convert back to 0-based indexing
	for (size_t i = 0; i < MAX_VERT_COUNT; i++)
	    outIndices[i] = indices[i] - 1;
    }
    if (outVertexData != NULL)
	memcpy(outVertexData, vData, nVerts*8*sizeof(float));

    return TRUE;
}


void SRM_DeleteMesh(SRM_Mesh *mesh)
    /* Frees the memory allocated by a mesh data structure. */
{
    mesh->isInit = FALSE;
    
    if (mesh->name != NULL) {
	free(mesh->name);
	mesh->name = NULL;
    }
    if (mesh->vertices != NULL) {
	free(mesh->vertices);
	mesh->vertices = NULL;
    }
    if (mesh->textureUVs != NULL) {
	free(mesh->textureUVs);
	mesh->textureUVs = NULL;
    }
    if (mesh->normals != NULL) {
	free(mesh->normals);
	mesh->normals = NULL;
    }
    if (mesh->faces != NULL) {
	free(mesh->faces);
	mesh->faces = NULL;
    }
    
    mesh->nFaces = 0;
    mesh->nNormals = 0;
    mesh->nVertices = 0;
    mesh->nTextureUVs = 0;
}


void SRM_PrintMesh(SRM_Mesh *mesh)
    /* Print the contents of the given mesh in human readable form. */
{
    if (!mesh->isInit)
	return;
    
    printf("Printing mesh data at %p.\n", (void*)mesh);
    if (mesh->name != NULL)
	printf("Name: %s\n", mesh->name);

    if (mesh->vertices != NULL) {
	printf("Vertices:\n");
	for (unsigned int i = 0; i < mesh->nVertices; i++) {
	    printf("%f, ", mesh->vertices[i]);
	    if (((i + 1) % 3) == 0)
		printf("\n");
	}
    }

    if (mesh->textureUVs != NULL) {
	printf("\nTextureUVs:\n");
	for (unsigned int i = 0; i < mesh->nTextureUVs; i++) {
	    printf("%f, ", mesh->textureUVs[i]);
	    if (((i + 1) % 2) == 0)
		printf("\n");
	}
    }

    if (mesh->normals != NULL) {
	printf("\nNormals:\n");
	for (unsigned int i = 0; i < mesh->nNormals; i++) {
	    printf("%f, ", mesh->normals[i]);
	    if (((i + 1) % 3) == 0)
		printf("\n");
	}
    }

    if (mesh->faces != NULL) {
	printf("\nFace Indices:\n");
	for (unsigned int i = 0; i < mesh->nFaces; i++) {
	    for (int j = 0; j < 3; j++) {
		for(int k = 0; k < 3; k++) {
		    printf("%ld/", mesh->faces[i].indices[(3*j)+k]);
		}
		printf(" ");
	    }
	    printf("\n");
	}
    }
}
