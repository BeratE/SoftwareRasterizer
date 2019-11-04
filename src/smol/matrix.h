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
    size_t cols;
    double *arr;
} SMOL_Matrix;


SMOL_Matrix* SMOL_CreateMatrix(size_t rows, size_t cols);
SMOL_Matrix* SMOL_CopyMatrix(const SMOL_Matrix *mat);
void         SMOL_FreeMatrix(SMOL_Matrix* mat);

void SMOL_FillNxMf(double* mat, size_t rows, size_t cols, double value);
void SMOL_FillMat(SMOL_Matrix *mat, double value);

void SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value);
double SMOL_GetIndex(const SMOL_Matrix* mat, size_t row, size_t col);

void SMOL_EyeNxNf(double *matOut, size_t size);
void SMOL_EyeMat(SMOL_Matrix *matOut);

void SMOL_Perspective4x4f(double *matOut);
void SMOL_PerspectiveMat(SMOL_Matrix *matOut);

void SMOL_Camera4x4f(const double* vec3pos, const double* vec3dir, const double* vec3up, double* matOut);
void SMOL_CameraMat(const SMOL_Matrix* vec3pos, const SMOL_Matrix* vec3dir,
		     const SMOL_Matrix* vec3up, SMOL_Matrix* matOut); 

void SMOL_MultiplyNMxMKf(const double *matA,  const double *matB, double *matOut,
			 size_t nA, size_t mAB, size_t kB);
void SMOL_MultiplyNxNf(const double *matA, const double *matB, double *matOut, size_t size);
void SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB, SMOL_Matrix *matOut);

void SMOL_ScalarMultiplyNxMf(const double *matA, double* matOut, size_t rows, size_t cols, double scalar);
void SMOL_ScalarMultiplyMat(const SMOL_Matrix *matA, SMOL_Matrix* matOut, double scalar);

double SMOL_VecDotNx1f(const double *vecA, const double *vecB, size_t rows);
double SMOL_VecDotMat(const SMOL_Matrix* vecA, const SMOL_Matrix* vecB);

void SMOL_VecCross3x1f(const double *vecA, const double* vecB, double *vecOut);
void SMOL_VecCrossMat(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB, SMOL_Matrix *vecOut);

void SMOL_AddNxMf(const double* matA, const double* matB, double* matCout,
		  size_t rows, size_t cols);
void SMOL_AddMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB, SMOL_Matrix* matOut);


void SMOL_SubtractNxMf(const double* matA, const double* matB, double* matOut,
		       size_t rows, size_t cols);
void SMOL_SubtractMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB, SMOL_Matrix* matOut);

// Projection

// Rotation

// Translation

// Scale

// Skew		 

#endif // MATRIX_H 
