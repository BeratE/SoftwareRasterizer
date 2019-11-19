#ifndef SMOL_MATRIX_H
#define SMOL_MATRIX_H

/* Simple Matrix Operation Library (smol). Small collection of handy Matrix operations.
 * Matrices are stored as one dimensional arrays of type double, row-major order. 
 * The (zero-indexed) index of a position in row i, column j is retrieved by [i*columns + j]. */

#include <stdlib.h>
#include <stdarg.h>

typedef struct {
    size_t nRows;
    size_t nCols;
    double *fields;
} SMOL_Matrix;

#define SMOL_NULLMATRIX (SMOL_Matrix){.nRows = 0, .nCols = 0, .fields = NULL}

SMOL_Matrix SMOL_AllocMatrix(size_t nRows, size_t nCols);
SMOL_Matrix SMOL_CopyMatrix(const SMOL_Matrix *mat);
SMOL_Matrix SMOL_RandomMatrix(size_t nRows, size_t nCols);
SMOL_Matrix SMOL_EyeMatrix(size_t size);
SMOL_Matrix SMOL_PerspectiveMatrix(double fov, double ratio, double near, double far); // TODO
SMOL_Matrix SMOL_CameraMatrix(const double* vec3pos, const double* vec3dir, const double* vec3up); // TODO

SMOL_Matrix SMOL_TransposedMat(SMOL_Matrix *mat);
SMOL_Matrix SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB);
SMOL_Matrix SMOL_CrossMat(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB);
SMOL_Matrix SMOL_AddMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB);
SMOL_Matrix SMOL_SubtractMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB);

void        SMOL_Fill(SMOL_Matrix *mat, double value);
void        SMOL_Scale(SMOL_Matrix *mat, double scalar);
void        SMOL_Translate(SMOL_Matrix *mat, const SMOL_Matrix *vecT);

void        SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value);
double      SMOL_GetIndex(const SMOL_Matrix* mat, size_t row, size_t col);

void        SMOL_Free(SMOL_Matrix* mat);
void        SMOL_FreeV(int count, ...);

#endif // SMOL_MATRIX_H 
