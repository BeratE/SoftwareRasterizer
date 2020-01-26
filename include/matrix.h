#ifndef SMOL_MATRIX_H
#define SMOL_MATRIX_H
/* Simple Matrix Operation Library (smol). Small collection of handy matrix operations. */

#include <stdarg.h>
#include <stdlib.h>

typedef struct SMOL_Matrix {
    size_t nRows;
    size_t nCols;
    double *fields;
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
int SMOL_PerspectiveMatrix(SMOL_Matrix* lhs, double fov, double ratio, double near, double far);
int SMOL_CameraMatrix(SMOL_Matrix* lhs, const double* vec3eye, const double* vec3front, const double* vec3up);

/* Elementary Row Operations */
int SMOL_SwapRows(SMOL_Matrix* lhs, size_t ri, size_t rj);
int SMOL_MultiplyRow(SMOL_Matrix* lhs, size_t row, double scalar);
int SMOL_AddRows(SMOL_Matrix *lhs, size_t dest_row, size_t src_row, double scalar);

/* Linear Equation Systems */
int SMOL_Echelon(SMOL_Matrix *lhs, size_t *outrank, int reduced);
int SMOL_Invert(SMOL_Matrix *lhs);
//int SMOL_SolveEquation(SMOL_Matrix *lhs, const SMOL_Matrix *coef, const SMOL_Matrix *b);

/* Basic Linear Operations */
int SMOL_Fill(SMOL_Matrix *lhs, double value);
int SMOL_Add(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_Subtract(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_Multiply(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_AddV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_SubtractV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_MultiplyV(SMOL_Matrix *lhs, size_t count, ...);
int SMOL_Transpose(SMOL_Matrix *lhs);
int SMOL_Scale(SMOL_Matrix *lhs, double scalar);
int SMOL_Rotate(SMOL_Matrix *lhs, const double* axis, double angle);
int SMOL_RotateXYZ(SMOL_Matrix *lhs, double angleX, double angleY, double angleZ);

/* Vector Operations */
int SMOL_VectorNormalize(SMOL_Matrix *lhs);
int SMOL_VectorCross(SMOL_Matrix *lhs, const SMOL_Matrix *rhs);
int SMOL_VectorLength(double* lhs, const SMOL_Matrix *vec);
int SMOL_VectorLentghSquare(double *lhs, const SMOL_Matrix *vec);
int SMOL_VectorDot(double* lhs, const SMOL_Matrix *vecA, const SMOL_Matrix *vecB);

/* Setters and Accessors */
int SMOL_SetField(SMOL_Matrix* lhs, size_t row, size_t col, double value);
int SMOL_SetRow(SMOL_Matrix *lhs, size_t row, const SMOL_Matrix *vec);
int SMOL_SetColumn(SMOL_Matrix *lhs, size_t col, const SMOL_Matrix *vec);
int SMOL_GetField(double *lhs, const SMOL_Matrix* mat, size_t row, size_t col);
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
