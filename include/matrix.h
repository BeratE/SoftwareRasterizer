#ifndef SMOL_MATRIX_H
#define SMOL_MATRIX_H
/* Simple Matrix Operation Library (smol). Small collection of handy Matrix operations. */

#include <stdarg.h>
#include <stdlib.h>

typedef struct {
    size_t nRows;
    size_t nCols;
    double *fields;
} SMOL_Matrix;

enum SMOL_TYPE {SMOL_TYPE_NULL, SMOL_TYPE_SCALAR, SMOL_TYPE_MATRIX,
		SMOL_TYPE_VECTOR, SMOL_TYPE_ROW_VECTOR, SMOL_TYPE_COLUMN_VECTOR};

#define SMOL_NULLMATRIX (SMOL_Matrix){.nRows = 0, .nCols = 0, .fields = NULL}

SMOL_Matrix SMOL_AllocMatrix(size_t nRows, size_t nCols);
SMOL_Matrix SMOL_RandomMatrix(size_t nRows, size_t nCols);
SMOL_Matrix SMOL_EyeMatrix(size_t size);
SMOL_Matrix SMOL_PerspectiveMatrix(double fov, double ratio, double near, double far);
SMOL_Matrix SMOL_CameraMatrix(const double* vec3eye, const double* vec3front, const double* vec3up);

SMOL_Matrix SMOL_CopyMat(const SMOL_Matrix *mat);
SMOL_Matrix SMOL_TransposeMat(const SMOL_Matrix *mat); 
SMOL_Matrix SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB);
SMOL_Matrix SMOL_AddMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB);
SMOL_Matrix SMOL_SubtractMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB);

SMOL_Matrix SMOL_CrossVec(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB);
double      SMOL_DotVec(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB);
double      SMOL_LengthVec(const SMOL_Matrix *vec);
double      SMOL_LentghSqVec(const SMOL_Matrix *vec);
void        SMOL_Normalize(SMOL_Matrix *vec);

void        SMOL_Fill(SMOL_Matrix *mat, double value);
void        SMOL_Scale(SMOL_Matrix *mat, double scalar);

int         SMOL_TypeOf(const SMOL_Matrix *mat);

void        SMOL_SetField(SMOL_Matrix* mat, size_t row, size_t col, double value);
void        SMOL_SetRow(SMOL_Matrix *mat, size_t row, const SMOL_Matrix *vec);
void        SMOL_SetColumn(SMOL_Matrix *mat, size_t col, const SMOL_Matrix *vec);
double      SMOL_GetField(const SMOL_Matrix* mat, size_t row, size_t col);
SMOL_Matrix SMOL_GetRow(const SMOL_Matrix *mat, size_t row);
SMOL_Matrix SMOL_GetColumn(const SMOL_Matrix *mat, size_t col);

void        SMOL_Free(SMOL_Matrix* mat);
void        SMOL_FreeV(int count, ...);

#endif // SMOL_MATRIX_H 
