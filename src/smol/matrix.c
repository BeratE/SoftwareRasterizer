#include "matrix.h"
#include <string.h>

SMOL_Matrix* SMOL_CreateMatrix(size_t rows, size_t cols)
/* Returns a newly allocated Matrix of given size and
 * initializes its values with zero.
 * ! Allocated memory has to be freed at some point.*/
{
    SMOL_Matrix *m = malloc(sizeof(SMOL_Matrix));
    m->rows = rows;
    m->cols = cols;
    size_t size = sizeof(double) * rows * cols;
    m->arr = malloc(size);
    memset(m->arr, 0.0, size);
    return m;
}

SMOL_Matrix* SMOL_CopyMatrix(const SMOL_Matrix *mat)
/* Returns a deep copy of given matrix. */
{
    SMOL_Matrix *m = malloc(sizeof(SMOL_Matrix));
    m->rows = mat->rows;
    m->cols = mat->cols;
    size_t size = sizeof(double) * m->rows * m->cols;
    m->arr = malloc(size);
    memcpy(m->arr, mat->arr, size);
    return m;
}

void SMOL_FreeMatrix(SMOL_Matrix* mat)
/* Free memory allocated by the matrix. */
{
    free(mat->arr);
    free(mat);
    mat = NULL;
}


void SMOL_FillNxMf(double* mat, size_t rows, size_t cols, double value)
/* Fils the given array with the given value. */
{
    memset(mat, value, sizeof(double) * rows * cols);
}

void SMOL_FillMat(SMOL_Matrix *mat, double value)
/* Fills the given matrix with the given value. */
{
    memset(mat->arr, value, sizeof(double) * mat->rows * mat->cols);
}


void SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value)
/* Sets the value of given matrix at position [row, col]. */
{
    mat->arr[row*mat->cols + col] = value;
}

double SMOL_GetIndex(const SMOL_Matrix* mat, size_t row, size_t col)
/* Returns the value of the matrix entry at [row, col]. */
{
    return mat->arr[row*mat->cols + col];
}



void SMOL_EyeNxNf(double *matOut, size_t size)
/* Assigns given matrix array to the identity matrix of size n. */
{
    memset(matOut, 0.0, sizeof(double) * size * size);
    for (unsigned int i = 0; i < size; i++) {
	matOut[i*size + i] = 1.0;
    }
}

void SMOL_EyeMat(SMOL_Matrix *mat)
/* Assigns given matrix of to the identity. */
{
    SMOL_EyeNxNf(mat->arr, mat->rows);
}


void SMOL_Perspective4x4f(double *matOut)
{
}

void SMOL_PerspectiveMat(SMOL_Matrix *matOut)
{
}

void SMOL_Camera4x4f(const double* vec3pos, const double* vec3dir, const double* vec3up, double* matOut)
/* Calculate the 4x4 view matrix from the given position, direction and up vector. */
{
    double* vec3newUp = malloc(sizeof(double) * 3);
    //SMOL_MultiplyNMxMKf(vec3dir, vec3dir, vec3newUp, 3, size_t mAB, size_t kB)
}

void SMOL_CameraMat(const SMOL_Matrix* vec3pos, const SMOL_Matrix* vec3dir,
		     const SMOL_Matrix* vec3up, SMOL_Matrix* matOut)
{
}


void SMOL_MultiplyNMxMKf(const double *matA, const double *matB, double *matCout,
			 size_t nA, size_t mAB, size_t kB)
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

void SMOL_MultiplyNxNf(const double *matA, const double *matB, double *matCout, size_t size)
/* Multiply two square matrix arrays of size N; A * B = C. */
{
    SMOL_MultiplyNMxMKf(matA, matB, matCout, size, size, size);
}

void SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB, SMOL_Matrix *matCout)
/* Multiply* two matrices of size NxM and MxK to a matrix NxK; A * B = C */
{
    SMOL_MultiplyNMxMKf(matA->arr, matB->arr, matCout->arr, matA->rows, matA->cols, matB->rows);
}

void SMOL_ScalarMultiplyNxMf(const double *matA, double* matOut, size_t rows, size_t cols, double scalar)
/* Multiply given matrix array with a scalar value. */
{
    for (unsigned int r = 0; r <rows; r++) {
	for (unsigned int c = 0; c < cols; c++)
	    matOut[r*cols+c] = scalar * matA[r*cols+c];
    }
}

void SMOL_ScalarMultiplyMat(const SMOL_Matrix *matA, SMOL_Matrix* matOut, double scalar)
/* Multiply given Matrix with a scalar value. */
{
    SMOL_ScalarMultiplyNxMf(matA->arr, matOut->arr, matA->rows, matA->cols, scalar);
}

double SMOL_VecDotNx1f(const double *vecA, const double *vecB, size_t rows)
/* Return the dot product between the two given vectors of given size. */
{
    double dot = 0.0;
    SMOL_MultiplyNMxMKf(vecA, vecB, &dot, 1, rows, 1);
    return dot;
}

double SMOL_VecDotMat(const SMOL_Matrix* vecA, const SMOL_Matrix* vecB)
/* Return the dot product of the two given vectors. */
{
    double dot = 0.0;
    SMOL_MultiplyNMxMKf(vecA->arr, vecB->arr, &dot, vecA->rows, 1, vecB->rows);
    return dot;
}

void SMOL_VecCross3x1f(const double *vecA, const double* vecB, double *vecOut)
/* Calculate the cross product of given vectors A and B; C = A x B. */ 
{
    vecOut[0] = vecA[1]*vecB[2] - vecA[2]*vecB[1];
    vecOut[1] = vecA[2]*vecB[0] - vecA[0]*vecB[3];
    vecOut[2] = vecA[0]*vecB[1] - vecA[1]*vecB[0];
}

void SMOL_VecCrossMat(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB, SMOL_Matrix *vecOut)
{
    SMOL_VecCross3x1f(vecA->arr, vecB->arr, vecOut->arr);
}


void SMOL_AddNxMf(const double* matA, const double* matB, double* matCout,
		  size_t rows, size_t cols)
/* Add two matrix arrays of the same size together; A + B = C. */
{
    for(size_t r = 0; r < rows; r++) {
	for (size_t c = 0; c < cols; c++) 
	    matCout[r*cols+c] = matA[r*cols+c] + matB[r*cols+c];
    }
}

void SMOL_AddMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB, SMOL_Matrix* matCout)
/* Add two matrices together; A + B = C. */
{
    SMOL_AddNxMf(matA->arr, matB->arr, matCout->arr, matA->rows, matA->cols);
}


void SMOL_SubtractNxMf(const double* matA, const double* matB, double* matCout,
		       size_t rows, size_t cols)
/* Subtract two matrix arrays of same size from eachother; A - B = C. */
{
     for(size_t r = 0; r < rows; r++) {
	for (size_t c = 0; c < cols; c++)
	    matCout[r*cols+c] = matA[r*cols+c] - matB[r*cols+c];
    }
}

void SMOL_SubtractMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB, SMOL_Matrix* matCout)
{
    SMOL_SubtractNxMf(matA->arr, matB->arr, matCout->arr, matA->rows, matA->cols);
}


