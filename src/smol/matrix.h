#ifndef MATRIX_H
#define MATRIX_H

/* Simple Matrix Operation Library (smol).
 * Small collection of handy Matrix operations.
 * Usage:
 * Matrices are stored as one dimensional arrays of type double, row-major order. 
 * The (zero-indexed) index of a position in row i, column j is retrieved by [i*columns + j]. 
 * Most functions are provided in one or more of the three following forms: 
 * *NxNf, Takes a size and performs computation on a 1d array interpreted as a square matrix.
 * *NXMf, Takes the number of rows N and Columns M and performs computation on the given 1d array.
 * *Mat,  Takes a pointer to a SMOL_Matrix struct. */

#include <stdlib.h>

/* Convienience struct of matrix elements and information. */
typedef struct {
    size_t rows;
    size_t columns;
    double *arr;
} SMOL_Matrix;


// Memory Allocation and release
SMOL_Matrix* SMOL_CreateMatrix(size_t rows, size_t cols);
void SMOL_FreeMatrix(SMOL_Matrix* mat);

// Convinient indexing
void SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value);
double SMOL_GetIndex(SMOL_Matrix* mat, size_t row, size_t col);

// Identity
void SMOL_EyeNxMf(size_t rows, size_t cols, double *matOut);
void SMOL_EyeNxNf(size_t size, double *matOut);
void SMOL_EyeMat(SMOL_Matrix *matOut);

// Multiplication
void SMOL_MultiplyNMxMKf(size_t nA, size_t mAB, size_t kB,
				const double *matA, const double *matB, double *matCout);
void SMOL_MultiplyNxNf(size_t size, const double *matA, const double *matB, double *matCout);
void SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB, SMOL_Matrix *matCout);

double SMOL_DotNf(size_t size, const double *vecA, const double *vecB);
double SMOL_DotMat(const SMOL_Matrix* vecA, const SMOL_Matrix* vecB);

// Addition
void SMOL_AddNxNf(size_t size, const double* matA, const double* matB, double* matCout);
void SMOL_AddMatf(const SMOL_Matrix* matA, const SMOL_Matrix* matB, SMOL_Matrix* matCout);


// Substraction

// Projection

// Rotation

// Translation

// Scale

// Skew		 

#endif // MATRIX_H 
