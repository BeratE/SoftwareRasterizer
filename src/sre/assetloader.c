#include "sre.h"
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

int SR_LoadMesh(SR_Mesh *scene, const char* filepath)
/* Return a mesh object created from the contents of the given file. */
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
	perror("Error opening file");
	return -1;
    }

    scene->faces = NULL;
    scene->normals = NULL;
    scene->vertices = NULL;
    scene->textureUVs = NULL;
    scene->nFaces = 0;
    scene->nNormals = 0;
    scene->nVertices = 0;
    scene->nTextureUVs = 0;
    
    char line[255];
    while (fgets(line, 255, file) != NULL) {
	char* lp = line;
	
	// skip empty lines and comments
	while (*lp == ' ')
	    lp++;
	if (*lp == '\0' || *lp == '#')
	    continue;

	if (strncmp(lp, "mtllib", 6) == 0) {
	    
	}
	
	else if (lp[0] == 'o') {
	    copyLineString(lp, &scene->name);
	}
	
	else if (lp[0] == 'f') {
	}

	// Vertex Data
	else if (lp[0] == 'v') {
	    lp++;
	    if (lp[0] == ' ') {
		collectLineFloat(lp, &scene->vertices, &scene->nVertices);
	    }
	    else if (lp[0] == 't') {
		collectLineFloat(lp, &scene->textureUVs, &scene->nTextureUVs);
	    }
	    else if (lp[0] == 'n') {
	     	collectLineFloat(lp, &scene->normals, &scene->nNormals);
	    }
	}
    }
    
    fclose(file);

    return 0;
}

