#include "minunit.h"
#include "sre/sre.h"

int main () {
    SR_Mesh mesh;
    SR_LoadMesh(&mesh, "cube2.obj");
    SR_PrintMesh(&mesh);
    return 0;
}
