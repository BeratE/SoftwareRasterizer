#include "minunit.h"
#include "matrix/matrix.h"

void setup ()
{
}

void teardown ()
{
}

MU_TEST(test_dot) {
    Matrix* m1 = MAT_CreateMatrix(1, 3);
    Matrix* m2 = MAT_CreateMatrix(1, 3);
    MAT_SetIndex(m1, 0, 0, 1.0);
    MAT_SetIndex(m2, 1, 1, 1.0);
    
    double dot = 0.0;
    MAT_MultiplyNMxMKf(1, 3, 1, m1->arr, m2->arr, &dot);

    mu_assert_double_eq(0.0, dot);
}

MU_TEST_SUITE(matrix_suite) {
    MU_SUITE_CONFIGURE(&setup, &teardown);
    MU_RUN_TEST(test_dot);
}

int main ()
{
    MU_RUN_SUITE(matrix_suite);
    MU_REPORT();
    return 0;
}
