#ifndef MATRIX_H
#define MATRIX_H
/* Matrices are two dimensional arrays of of row-major order.
 * Vectors are a special form of matric of width 1.
 * Vectors are stored as one dimensional arrays (or rows of matrices). */

#include <stdint.h>

// Matrix
void MAT_EyeNf(const size_t size, double **mat);
void MAT_MultiplyNxNf(const size_t size, const double **matA, const double **matB, double **matCout);

// Vector
double MAT_DotNf(const size_t size, const double *vecA, const double *vecB);

#endif // MATRIX_H 
