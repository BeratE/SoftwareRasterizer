#include "minunit.h"
#include "sre/srmesh.h"

int main () {
    SRM_Mesh mesh;
    SRM_LoadMesh(&mesh, "/home/berat/Projects/cepples/rtg/assets/cube.obj");
    SRM_PrintMesh(&mesh);

    // Collect Indexed Mesh Vertex Data
    size_t vertexCount;
    SRM_IndexedMeshVertexData(&mesh, NULL, NULL, &vertexCount);

    const size_t INDEX_COUNT = mesh.nFaces * 3;
    const size_t VDATA_COUNT = vertexCount * 8;
    size_t indices[INDEX_COUNT];
    float vertexData[VDATA_COUNT];
    
    SRM_IndexedMeshVertexData(&mesh, vertexData, indices, NULL);

    printf("\nIndices:\n");
    for (size_t i = 0; i < INDEX_COUNT; i++) {
	printf("%ld", indices[i]);
	if (!((i+1)%6))
	    printf("\n");
	else
	    printf(", ");
    }

    printf("\nVertex Data:\n");
    for (size_t i = 0; i < VDATA_COUNT; i++) {
	printf("%.1f\t", vertexData[i]);
	if (!((i+1)%8))
	    printf("\n");
	else
	    printf(", ");
    }

    
    return 0;
}
