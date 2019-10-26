#include "matrix.h"
#include <string.h>

//
// Matrix Creation
//
SMOL_Matrix* SMOL_CreateMatrix(size_t rows, size_t cols)
/* Returns a newly allocated Matrix of given size and
 * initializes its values with zero.
 * ! Allocated memory has to be freed at some point.*/
{
    SMOL_Matrix *m = malloc(sizeof(SMOL_Matrix));
    m->rows = rows;
    m->columns = cols;
    size_t size = sizeof(double) * rows * cols;
    m->arr = malloc(size);
    memset(m->arr, 0.0, size);
    return m;
}

void SMOL_FreeMatrix(SMOL_Matrix* mat)
/* Free memory allocated by the matrix. */
{
    free(mat->arr);
    free(mat);
    mat = NULL;
}

//
// Matrix Modification
//
void SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value)
/* Sets the value of given matrix at position [row, col]*/
{
    mat->arr[row*mat->columns + col] = value;
}

double SMOL_GetIndex(SMOL_Matrix* mat, size_t row, size_t col)
/* Returns the value of the matrix entry at [row, col}*/
{
    return mat->arr[row*mat->columns + col];
}

void SMOL_EyeNxMf(size_t rows, size_t cols, double *matOut)
/* Assigns given matrix array to the (cutoff) identity of size n = height, m = width. */
{
    memset(matOut, 0.0, sizeof(double) * rows * cols);
    size_t m = (rows < cols) ? rows : cols;
    for (unsigned int i = 0; i < m; i++) {
	matOut[i*rows + i] = 1.0;
    }
}

void SMOL_EyeNxNf(size_t size, double *matOut)
/* Assigns given matrix array to the identity matrix of size n. */
{
    SMOL_EyeNxMf(size, size, matOut);
}

void SMOL_EyeMat(SMOL_Matrix *mat)
/* Assigns given matrix of variable size to the (cutoff) identity. */
{
    SMOL_EyeNxMf(mat->rows, mat->columns, mat->arr);
}

//
// Matrix Multiplication
//
void SMOL_MultiplyNMxMKf(size_t nA, size_t mAB, size_t kB,
		       const double *matA, const double *matB, double *matCout)
/* Multiply two matrix arrays of of NMxMK size; A * B = C. 
 * Matrix Array C has to be of size NxK. */
{
    memset(matCout, 0.0, sizeof(double) * nA * kB);
    for (unsigned int i = 0; i < nA; i++) {
	for (unsigned int j = 0; j < kB; j++) {
	    for (unsigned int l = 0; l < mAB; l++)
		matCout[i*kB + j] += matA[i*mAB + l] * matB[l*kB + j];
	}
    }
}

void SMOL_MultiplyNxNf(size_t size, const double *matA, const double *matB, double *matCout)
/* Multiply two square matrix arrays of size N; A * B = C. */
{
    SMOL_MultiplyNMxMKf(size, size, size, matA, matB, matCout);
}

void SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB, SMOL_Matrix *matCout)
/* Multiply* two matrices of size NxM and MxK to a matrix NxK; A * B = C */
{
    SMOL_MultiplyNMxMKf(matA->rows, matA->columns, matB->rows, matA->arr, matB->arr, matCout->arr);
}

double SMOL_DotNf(size_t size, const double *vecA, const double *vecB)
/* Return the dot product between the two given vectors of given size. */
{
    double dot = 0.0;
    SMOL_MultiplyNMxMKf(1, size, 1, vecA, vecB, &dot);
    return dot;
}

double SMOL_DotMat(const SMOL_Matrix* vecA, const SMOL_Matrix* vecB)
/* Return the dot product of vector A and vector B. */
{

}
