#include "minunit.h"
#include "smol/matrix.h"

void setup ()
{
}

void teardown ()
{
}

MU_TEST(dotproduct) {
    SMOL_Matrix v1t = SMOL_AllocMatrix(1, 2);
    SMOL_Matrix v2  = SMOL_AllocMatrix(2, 1);
    
    SMOL_SetField(&v1t, 0, 0, 0.5);
    SMOL_SetField(&v1t, 0, 1, 0.5);
    
    SMOL_SetField(&v2, 0, 0, -0.5);
    SMOL_SetField(&v2, 1, 0, 0.5);
    
    SMOL_Matrix r1 = SMOL_MultiplyMat(&v1t, &v2);
    mu_assert_int_eq(r1.nRows, 1);
    mu_assert_int_eq(r1.nCols, 1);
    mu_assert_double_eq(r1.fields[0], 0.0);

    SMOL_FreeV(3, &v1t, &v2, &r1);
}

MU_TEST(addition) {
    SMOL_Matrix m1 = SMOL_EyeMatrix(4);
    SMOL_Matrix m2 = SMOL_AllocMatrix(4, 4);
    SMOL_Matrix m3 = SMOL_AllocMatrix(2, 4);

    for (int r = 0; r < 4; r++) {
	for (int c = 0; c < 4; c++)
	    SMOL_SetField(&m2, r, c, r+c);
    }
    // Sucessfull addition
    SMOL_Matrix r1 = SMOL_AddMat(&m1, &m2);
    for (int r = 0; r < 4; r++) {
	for (int c = 0; c < 4; c++) {
	    double ex = r + c;
	    if (r == c) 
		ex++;
	    mu_assert_double_eq(ex, SMOL_GetField(&r1, r, c));
	}
    }

    // Error
    SMOL_Matrix r2 = SMOL_AddMat(&m1, &m3);
    mu_check(r2.fields == NULL);

    SMOL_FreeV(5, &m1, &m2, &m3, &r1, &r2);
}

MU_TEST(multiplication) {
    double a1[] = { 2.0, -2.0, 1.0,
		    5.0, -7.0, 3.0};
    double a2[] = { 0.0, 5.0,
		    3.0, 7.0,
		    7.0, 4.0};
    SMOL_Matrix m1 = (SMOL_Matrix){.nRows=2,.nCols=3,.fields=a1};
    SMOL_Matrix m2 = (SMOL_Matrix){.nRows=3,.nCols=2,.fields=a2};

    SMOL_Matrix r1 = SMOL_MultiplyMat(&m1, &m2);

    mu_assert_int_eq(2, r1.nRows);
    mu_assert_int_eq(2, r1.nCols);
    mu_assert_double_eq(1.0,  SMOL_GetField(&r1, 0, 0));
    mu_assert_double_eq(0.0,  SMOL_GetField(&r1, 0, 1));
    mu_assert_double_eq(0.0,  SMOL_GetField(&r1, 1, 0));
    mu_assert_double_eq(-12.0, SMOL_GetField(&r1, 1, 1));

    SMOL_FreeV(1, &r1);
}

MU_TEST(crossproduct) {
    double a1[] = { 4.0, 5.0, 7.0};
    double a2[] = { 3.0, 2.0, 0.5};

    SMOL_Matrix m1 = (SMOL_Matrix){.nRows=3,.nCols=1,.fields=a1};
    SMOL_Matrix m2 = (SMOL_Matrix){.nRows=3,.nCols=1,.fields=a2};
    SMOL_Matrix m3 = SMOL_CrossVec(&m1, &m2);
    
    mu_assert(m3.fields != NULL, "");
    mu_assert_double_eq(-11.5, SMOL_GetField(&m3, 0, 0));
    mu_assert_double_eq( 19,   SMOL_GetField(&m3, 1, 0));
    mu_assert_double_eq(-7,   SMOL_GetField(&m3, 2, 0));
    
    /*    m1.nRows = 1;
    m1.nCols = 3;
    SMOL_Matrix d1 = SMOL_MultiplyMat(&m1, &m3);
    mu_assert(d1.fields != NULL, "");
    mu_assert_int_eq(d1.nRows, 1);
    mu_assert_int_eq(d1.nCols, 1);
    mu_assert_double_eq(0.0, *d1.fields);
    */
    SMOL_Free(&m3);
}

MU_TEST_SUITE(matrix_suite) {
    MU_SUITE_CONFIGURE(&setup, &teardown);
    MU_RUN_TEST(addition);
    MU_RUN_TEST(multiplication);
    MU_RUN_TEST(dotproduct);
    MU_RUN_TEST(crossproduct);
}

int main ()
{
    MU_RUN_SUITE(matrix_suite);
    MU_REPORT();
    return 0;
}
