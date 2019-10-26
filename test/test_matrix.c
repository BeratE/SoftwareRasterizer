#include "minunit.h"
#include "smol/matrix.h"

void setup ()
{
}

void teardown ()
{
}

MU_TEST(test_dot_mul) {
    SMOL_Matrix* m1 = SMOL_CreateMatrix(3, 1);
    SMOL_Matrix* m2 = SMOL_CreateMatrix(3, 1);
    SMOL_SetIndex(m1, 0, 0, 1.0);
    SMOL_SetIndex(m2, 1, 1, 1.0);
    
    double dot1 = 0.0;
    SMOL_MultiplyNMxMKf(1, 3, 1, m1->arr, m2->arr, &dot1);
    double dot2 = SMOL_DotNf(3, m1->arr, m2->arr);

    SMOL_FreeMatrix(m1);
    SMOL_FreeMatrix(m2);

    mu_assert_double_eq(0.0, dot1);
    mu_assert_double_eq(0.0, dot2);
}

MU_TEST(test_mat_mul) {
    SMOL_Matrix* m1 = SMOL_CreateMatrix(3, 1);
    SMOL_Matrix* m2 = SMOL_CreateMatrix(1, 3);
    SMOL_Matrix* m3 = SMOL_CreateMatrix(3, 3);

    SMOL_SetIndex(m1, 2, 0, 1.0);
    SMOL_SetIndex(m2, 0, 2, 1.0);
    
    SMOL_MultiplyNMxMKf(3, 1, 3, m1->arr, m2->arr, m3->arr);

    mu_assert_double_eq(1.0, SMOL_GetIndex(m3, 2, 2));
    mu_assert_double_eq(0.0, SMOL_GetIndex(m3, 0, 0));   
    
    SMOL_FreeMatrix(m1);
    SMOL_FreeMatrix(m2);
    SMOL_FreeMatrix(m3);
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
