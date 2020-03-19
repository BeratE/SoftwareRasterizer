#include "sre.h"
#include "types.h"
#include <string.h>
#include <stdio.h>

static void collectLineFloat(char *lp, float **array, size_t *num)
{
    while(*(++lp) == ' ');;
    while (*lp != '\0') {
	(*array) = realloc((*array), ((*num)+1)*sizeof(float));
	(*array)[*num] = atof(lp);
	(*num)++;
	while(*(++lp) != ' ' && *lp != '\0');;
    }
}

static void copyLineString(char *lp, char** str)
{
    while(*(++lp) == ' ');;
    (*str) = malloc(strlen(lp) + 1);
    strcpy((*str), lp);    
}

int SR_LoadMesh(SR_Mesh *mesh, const char *filepath)
/* Return a mesh object created from the contents of the given file. */
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
	while (*lp == ' ')
	    lp++;
	if (*lp == '\0' || *lp == '#')
	    continue;

	if (strncmp(lp, "mtllib", 6) == 0) {

	}

	else if (lp[0] == 'o') {
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
	    while(*(++lp) == ' ');;
	    mesh->faces = realloc(mesh->faces, (mesh->nFaces+1)*sizeof(SR_Face));
	    SR_Face *face = &mesh->faces[mesh->nFaces];
	    
	    face->primitiveType = 0;
	    face->indices = NULL;
	    while(*lp != '\0') {
		face->primitiveType++;
		face->indices = realloc(face->indices, 3*face->primitiveType*sizeof(int));

		for (int i = 0; i < 3; i++) {
		    face->indices[3*(face->primitiveType-1)+i] = atoi(lp);

		    while(*lp != '/' && *lp != ' ' && *lp != '\0' && lp++);;
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
