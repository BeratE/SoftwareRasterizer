#include "minunit.h"
#include "bemo/bemo.h"

int main () {
    BEMO_Mesh scene;
    BEMO_LoadMesh(&scene, "cube.obj");
    return 0;
}
