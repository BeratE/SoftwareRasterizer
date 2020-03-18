#ifndef SMOL_MATRIX_H
#define SMOL_MATRIX_H
/* Simple Matrix Operation Library (smol). Small collection of handy matrix operations. */

#include <stdarg.h>
#include <stdlib.h>

typedef struct SMOL_Matrix {
    size_t nRows;
    size_t nCols;
    float *fields;
} SMOL_Matrix;

/* Enums */
enum SMOL_TYPE {SMOL_TYPE_NULL,
		SMOL_TYPE_SCALAR,
		SMOL_TYPE_MATRIX,
		SMOL_TYPE_VECTOR,
		SMOL_TYPE_ROW_VECTOR,
		SMOL_TYPE_COLUMN_VECTOR};

enum SMOL_STATUS {SMOL_STATUS_OK,
		  SMOL_STATUS_NO_VALUE,
		  SMOL_STATUS_INVALID_TYPE,
		  SMOL_STATUS_INVALID_ARGUMENTS,
		  SMOL_STATUS_INCOMPATIBLE_SIZES,
		  SMOL_STATUS_ARRAY_OUT_OF_BOUNDS};

/* Macros */ 
#define SMOL_NULLMATRIX (SMOL_Matrix){.nRows = 0, .nCols = 0, .fields = NULL}

/* Matrix Allocation */
int SMOL_AllocMatrix(SMOL_Matrix* lhs, size_t nRows, size_t nCols);
int SMOL_RandomMatrix(SMOL_Matrix *lhs, size_t nRows, size_t nCols, size_t mod);
int SMOL_CopyMatrix(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_CopySubMatrix(SMOL_Matrix *lhs, const SMOL_Matrix *rhs, size_t r0, size_t c0, size_t r1, size_t c1);
int SMOL_EyeMatrix(SMOL_Matrix *lhs, size_t size);
int SMOL_PerspectiveMatrix(SMOL_Matrix* lhs, float fov, float ratio, float near, float far);
int SMOL_CameraMatrix(SMOL_Matrix* lhs, const float* vec3eye, const float* vec3front, const float* vec3up);
int SMOL_RotationMatrix(SMOL_Matrix *lhs, const float* axis, float angle);

/* Elementary Row Operations */
int SMOL_SwapRows(SMOL_Matrix* lhs, size_t ri, size_t rj);
int SMOL_MultiplyRow(SMOL_Matrix* lhs, size_t row, float scalar);
int SMOL_AddRows(SMOL_Matrix *lhs, size_t dest_row, size_t src_row, float scalar);

/* Linear Equation Systems */
int SMOL_Echelon(SMOL_Matrix *lhs, size_t *outrank, int reduced);
int SMOL_Invert(SMOL_Matrix *lhs);

/* Basic Operations */
int SMOL_Fill(SMOL_Matrix *lhs, float value);
int SMOL_Add(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_AddV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_Subtract(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_SubtractV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_Multiply(SMOL_Matrix *lhs, const SMOL_Matrix *rhsA, const SMOL_Matrix *rhsB);
int SMOL_MultiplyV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_Transpose(SMOL_Matrix *lhs);
int SMOL_Scale(SMOL_Matrix *lhs, float scalar);

/* Vector Operations */
int SMOL_VectorNormalize(SMOL_Matrix *lhs);
int SMOL_VectorCross(SMOL_Matrix *lhs, const SMOL_Matrix *rhsA, const SMOL_Matrix *rhsB);
int SMOL_VectorLength(float* lhs, const SMOL_Matrix *vec);
int SMOL_VectorLentghSquare(float *lhs, const SMOL_Matrix *vec);
int SMOL_VectorDot(float* lhs, const SMOL_Matrix *vecA, const SMOL_Matrix *vecB);

/* Setters and Accessors */
int SMOL_SetField(SMOL_Matrix* lhs, size_t row, size_t col, float value);
int SMOL_SetRow(SMOL_Matrix *lhs, size_t row, const SMOL_Matrix *vec);
int SMOL_SetColumn(SMOL_Matrix *lhs, size_t col, const SMOL_Matrix *vec);
int SMOL_GetField(float *lhs, const SMOL_Matrix* mat, size_t row, size_t col);
int SMOL_GetRow(SMOL_Matrix *lhs, const SMOL_Matrix *mat, size_t row);
int SMOL_GetColumn(SMOL_Matrix *lhs, const SMOL_Matrix *mat, size_t col);

/* Matrix Manipulation */
int SMOL_AppendRows(SMOL_Matrix* lhs, const SMOL_Matrix *rhs);
int SMOL_AppendColumns(SMOL_Matrix* lhs, const SMOL_Matrix *rhs);

/* Misc. functions */
int SMOL_TypeOf(const SMOL_Matrix *mat);
void SMOL_PrintMatrix(const SMOL_Matrix* mat);
void SMOL_PrintError(enum SMOL_STATUS status);
void SMOL_Free(SMOL_Matrix* mat);
void SMOL_FreeV(int count, ...);

#endif // SMOL_MATRIX_H 
