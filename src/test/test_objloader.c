#include "minunit.h"
#include "sre/sre.h"

int main () {
    SR_Mesh mesh;
    SR_LoadMesh(&mesh, "cube2.obj");

    printf("Name: %s", mesh.name);

    printf("\nVertices:\n");
    for (unsigned int i = 0; i < mesh.nVertices; i++) {
	printf("%f, ", mesh.vertices[i]);
	if (((i + 1) % 3) == 0)
	    printf("\n");
    }
    
    printf("\nTextureUVs:\n");
    for (unsigned int i = 0; i < mesh.nTextureUVs; i++) {
	printf("%f, ", mesh.textureUVs[i]);
	if (((i + 1) % 2) == 0)
	    printf("\n");
    }
    
    printf("\nNormals:\n");
    for (unsigned int i = 0; i < mesh.nNormals; i++) {
	printf("%f, ", mesh.normals[i]);
	if (((i + 1) % 3) == 0)
	    printf("\n");
    }
    
    printf("\nFace Indices:\n");
    for (unsigned int i = 0; i < mesh.nFaces; i++) {
	for (int j = 0; j < mesh.faces[i].primitiveType; j++) {
	    for(int k = 0; k < 3; k++) {
		printf("%d/", mesh.faces[i].indices[(3*j)+k]);
	    }
	    printf(" ");
	}
	printf("\n");
    }

    return 0;
}
