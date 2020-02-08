#include "objloader.h"
#include <stdio.h>

int SR_OBJ_LoadScene(SR_OBJ_Scene *scene, const char* filepath)
/* Return a scene object created from the contents of the given file. */
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
	perror("Error opening file");
	return -1;
    }
    
    char line[255];
    while (fgets(line, 255, file) != NULL) {
	puts(line);
    }
    
    fclose(file);

    return 0;
}
