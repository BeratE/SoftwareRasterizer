#include "bemo.h"
#include <string.h>
#include <stdio.h>

int BEMO_LoadMesh(BEMO_Mesh *scene, const char* filepath)
/* Return a mesh object created from the contents of the given file. */
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
	perror("Error opening file");
	return -1;
    }
    
    char line[255];
    while (fgets(line, 255, file) != NULL) {
	//ignore any whitespace, skip empty lines
	char* lp = line;
	while(*lp != '\0' && *lp == ' ')
	    lp++;
	if (*lp == '\0')
	    continue;

	if (lp[0] == 'o') {
	    while(*lp != '\0' && *lp == ' ')
		lp++;

	    scene->name = malloc(strlen(lp) + 1);
	    strcpy(scene->name, lp);

	    puts(scene->name);
	}
	
	if (lp[0] == 'v') {
	    if (lp[1] == ' ') {

	    }
	    if (lp[1] == 't') {

	    }
	    if (lp[1] == 'n') {
		    
	    }
	}
    }
    
    fclose(file);

    return 0;
}

