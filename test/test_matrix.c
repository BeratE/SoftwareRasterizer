#include "minunit.h"
#include "matrix/matrix.h"

void setup ()
{
}

void teardown ()
{
}

MU_TEST(test_dot_mul) {
    Matrix* m1 = MAT_CreateMatrix(3, 1);
    Matrix* m2 = MAT_CreateMatrix(3, 1);
    MAT_SetIndex(m1, 0, 0, 1.0);
    MAT_SetIndex(m2, 1, 1, 1.0);
    
    double dot = 0.0;
    MAT_MultiplyNMxMKf(1, 3, 1, m1->arr, m2->arr, &dot);

    MAT_FreeMatrix(m1);
    MAT_FreeMatrix(m2);
    
    mu_assert_double_eq(0.0, dot);
}

MU_TEST(test_mat_mul) {
    Matrix* m1 = MAT_CreateMatrix(3, 1);
    Matrix* m2 = MAT_CreateMatrix(1, 3);
    Matrix* m3 = MAT_CreateMatrix(3, 3);

    MAT_SetIndex(m1, 2, 0, 1.0);
    MAT_SetIndex(m2, 0, 2, 1.0);
    
    MAT_MultiplyNMxMKf(3, 1, 3, m1->arr, m2->arr, m3->arr);

    mu_assert_double_eq(1.0, MAT_GetIndex(m3, 2, 2));
    mu_assert_double_eq(0.0, MAT_GetIndex(m3, 0, 0));   
    
    MAT_FreeMatrix(m1);
    MAT_FreeMatrix(m2);
    MAT_FreeMatrix(m3);
}

MU_TEST_SUITE(matrix_suite) {
    MU_SUITE_CONFIGURE(&setup, &teardown);
    MU_RUN_TEST(test_dot_mul);
    MU_RUN_TEST(test_mat_mul);
}

int main ()
{
    MU_RUN_SUITE(matrix_suite);
    MU_REPORT();
    return 0;
}
