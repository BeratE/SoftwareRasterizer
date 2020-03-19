#include "minunit.h"
#include "sre/sre.h"

int main () {
    SR_Mesh scene;
    SR_LoadMesh(&scene, "cube.obj");

    printf("Name: %s", scene.name);

    printf("\nVertices:\n");
    for (unsigned int i = 0; i < scene.nVertices; i++) {
	printf("%f, ", scene.vertices[i]);
	if (((i+1) % 3) == 0)
	    printf("\n");
    }
    printf("\nTextureUVs:\n");
    for (unsigned int i = 0; i < scene.nTextureUVs; i++) {
	printf("%f, ", scene.textureUVs[i]);
	if (((i+1) % 2) == 0)
	    printf("\n");
    }
    printf("\nNormals:\n");
    for (unsigned int i = 0; i < scene.nNormals; i++) {
	printf("%f, ", scene.normals[i]);
	if (((i+1) % 3) == 0)
	    printf("\n");
    }
    
    return 0;
}
