#include "sre.h"
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


int SR_LoadMesh(SR_Mesh *mesh, const char *filepath)
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
	    mesh->faces = realloc(mesh->faces, (mesh->nFaces+1)*sizeof(SR_Face));
	    SR_Face *face = &mesh->faces[mesh->nFaces];
	    face->primitiveType = 0;
	    face->indices = NULL;
	    
	    while(*lp != '\0') {
		face->primitiveType++;
		face->indices = realloc(face->indices, 3*face->primitiveType*sizeof(int));

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


int SR_PrintMesh(SR_Mesh *mesh)
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
		printf("%d/", mesh->faces[i].indices[(3*j)+k]);
	    }
	    printf(" ");
	}
	printf("\n");
    }

    return 0;
}


int SR_IndexedMeshVertexData(SR_Mesh *mesh, float *vertexData, size_t *indices)
{
    vertexData = malloc((mesh->nVertices+mesh->nTextureUVs+mesh->nNormals) * sizeof(float));
    indices = malloc(mesh->faces->primitiveType * mesh->nFaces * sizeof(size_t));

    return 0;
}
