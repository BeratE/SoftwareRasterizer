#ifndef MATRIX_H
#define MATRIX_H
/* Matrices are stored as one dimensional arrays of row-major order. 
 * The (zero-indexed) index of a position in row i, column j is retrieved by [i*columns + j]. */

#include <stdlib.h>

typedef struct {
    size_t rows;
    size_t columns;
    double *arr;
} Matrix;


// Matrix
Matrix* MAT_CreateMatrix(size_t rows, size_t cols);
void MAT_FreeMatrix(Matrix* mat);

void MAT_SetIndex(Matrix* mat, size_t row, size_t col, double value);
double MAT_GetIndex(Matrix* mat, size_t row, size_t col);

void MAT_EyeNxMf(size_t rows, size_t cols, double *matOut);
void MAT_EyeNxNf(size_t size, double *matOut);
void MAT_EyeMat(Matrix *matOut);

void MAT_MultiplyNMxMKf(size_t nA, size_t mAB, size_t kB,
		       const double *matA, const double *matB, double *matCout);
void MAT_MultiplyNxNf(size_t size, const double *matA, const double *matB, double *matCout);
void MAT_MultiplyMat(const Matrix *matA, const Matrix *matB, Matrix *matCout);

// Vector
double MAT_DotNf(size_t size, const double *vecA, const double *vecB);

#endif // MATRIX_H 
