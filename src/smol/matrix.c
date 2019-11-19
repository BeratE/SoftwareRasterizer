#include "matrix.h"
#include <string.h>
#include <time.h>

static unsigned long _randseed = 0;

SMOL_Matrix SMOL_AllocMatrix(size_t nRows, size_t nCols)
/* Returns a newly allocated Matrix of given size and
 * initializes its values with zero.
 * ! Allocated memory has to be freed at some point.*/
{
    SMOL_Matrix m;
    m.nRows = nRows;
    m.nCols = nCols;
    const size_t size = sizeof(double) * nRows * nCols;
    m.fields = malloc(size);
    return m;
}

SMOL_Matrix SMOL_CopyMatrix(const SMOL_Matrix *mat)
/* Returns a deep copy of given matrix. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(mat->nRows, mat->nCols);
    memcpy(m.fields, mat->fields, sizeof(double) * mat->nRows * mat->nCols);
    return m;
}

SMOL_Matrix SMOL_RandomMatrix(size_t nRows, size_t nCols)
/* Creates a matrix of given size and fills it with random values. */
{
    if (_randseed == 0) { // Init seed
	_randseed = ((unsigned)time(NULL)/2);
	srand(_randseed);
    }
    
    SMOL_Matrix m = SMOL_AllocMatrix(nRows, nCols);;

    for (size_t r = 0; r < nRows; r++) {
	for (size_t c = 0; c < nCols; c++) {
	    m.fields[r*nCols+c] = rand();
	}
    }
    
    return m;
}

SMOL_Matrix SMOL_EyeMatrix(size_t size)
/* Assigns given matrix of to the identity. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(size, size);
    for (unsigned int i = 0; i < size; i++)
	SMOL_SetIndex(&m, i, i, 1.0);
    return m;
}

SMOL_Matrix SMOL_PerspectiveMatrix(double fov, double ratio, double near, double far)
/* Construct the perspective matrix from the given parameters. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(4, 4);

    return m;
}

SMOL_Matrix SMOL_CameraMatrix(const double* vec3pos, const double* vec3dir, const double* vec3up)
/* Construct the camera matrix from the given vector arrays. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(4, 4);

    return m;
}



SMOL_Matrix SMOL_TransposedMat(SMOL_Matrix *mat)
/* Return the transposed matrix of the given matrix; A = A^T. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(mat->nCols, mat->nRows);
    for (unsigned int r = 0; r < mat->nRows; r++) {
	for (unsigned int c = 0; c < mat->nCols; c++) {
	    SMOL_SetIndex(&m, r, c, SMOL_GetIndex(mat, c, r));
	}
    }
    return m;
}

SMOL_Matrix SMOL_MultiplyMat(const SMOL_Matrix *matA, const SMOL_Matrix *matB)
/* Multiply* two matrices of size NxM and MxK to a matrix NxK; C = A * B. */
{
    SMOL_Matrix m = SMOL_NULLMATRIX;
    if (matA->nCols != matB->nRows)
	return m;
    
    m = SMOL_AllocMatrix(matA->nRows, matB->nCols);
    for (unsigned int rA = 0; rA < matA->nRows; rA++) {
	for (unsigned int cB = 0; cB < matB->nCols; cB++) {
	    for (unsigned int i = 0; i < matA->nCols; i++)
		m.fields[rA*matB->nCols+cB] += matA->fields[rA*matA->nCols+i]*matB->fields[i*matB->nCols+cB];
	}
    }
    return m;
}

SMOL_Matrix SMOL_CrossMat(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB)
/* Calculate the cross product of given vectors A and B; C = A x B. */ 
{
    SMOL_Matrix m = SMOL_NULLMATRIX;
    if (vecA->nCols != 1 || vecB->nCols != 1 || vecA->nRows != 3 || vecB->nRows != 3)
	return m;

    m = SMOL_AllocMatrix(3, 1);
    m.fields[0] = vecA->fields[1]*vecB->fields[2] - vecA->fields[2]*vecB->fields[1];
    m.fields[1] = vecA->fields[2]*vecB->fields[0] - vecA->fields[0]*vecB->fields[2];
    m.fields[2] = vecA->fields[0]*vecB->fields[1] - vecA->fields[1]*vecB->fields[0];
    
    return m;
}

SMOL_Matrix SMOL_AddMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB)
/* Add two matrices of same size together; C = A + B. */
{
    SMOL_Matrix m = SMOL_NULLMATRIX;
    if (matA->nRows != matB->nRows || matA->nCols != matB->nCols)
	return m;

    m = SMOL_AllocMatrix(matA->nRows, matA->nCols);
    for(size_t r = 0; r < matA->nRows; r++) {
	for (size_t c = 0; c < matA->nCols; c++) 
	    m.fields[r*matA->nCols+c] = matA->fields[r*matA->nCols+c] + matB->fields[r*matA->nCols+c];
    }

    return m;
}

SMOL_Matrix SMOL_SubtractMat(const SMOL_Matrix* matA, const SMOL_Matrix* matB)
/* Subtact two matrices of same size from each other; C = A - B. */
{
    SMOL_Matrix m = SMOL_NULLMATRIX;
    if (matA->nRows != matB->nRows || matA->nCols != matB->nCols)
	return m;

    m = SMOL_AllocMatrix(matA->nRows, matA->nCols);
    for(size_t r = 0; r < matA->nRows; r++) {
	for (size_t c = 0; c < matA->nCols; c++) 
	    m.fields[r*matA->nCols+c] = matA->fields[r*matA->nCols+c] - matB->fields[r*matA->nCols+c];
    }

    return m;
}



void SMOL_Fill(SMOL_Matrix *mat, double value)
/* Fills the given matrix with the given value. */
{
    memset(mat->fields, value, sizeof(double) * mat->nRows * mat->nCols);
}

void SMOL_Scale(SMOL_Matrix *mat, double scalar)
/* Multiply given Matrix with a scalar value. */
{
    for (unsigned int r = 0; r < mat->nRows; r++) {
	for (unsigned int c = 0; c < mat->nCols; c++)
	    mat->fields[r*mat->nCols+c] *= scalar;
    }
}

void SMOL_Translate(SMOL_Matrix *mat, const SMOL_Matrix *vecT)
/* Translate a given matrix by the translation vector. */
{
    for (size_t r = 0; r < vecT->nRows; r++) {
	mat->fields[r*mat->nCols+(mat->nCols-1)] = vecT->fields[r];
    }
}



void SMOL_SetIndex(SMOL_Matrix* mat, size_t row, size_t col, double value)
/* Sets the value of given matrix at position [row, col]. */
{
    mat->fields[row*mat->nCols + col] = value;
}

double SMOL_GetIndex(const SMOL_Matrix* mat, size_t row, size_t col)
/* Returns the value of the matrix entry at [row, col]. */
{
    return mat->fields[row*mat->nCols + col];
}



void SMOL_Free(SMOL_Matrix* mat)
/* Free memory allocated by the matrix. */
{
    if (mat->fields != NULL) {
	free(mat->fields);
	mat->fields = NULL;
	mat->nRows = 0;
	mat->nCols = 0;
    }
}

void SMOL_FreeV(int count, ...)
/* Free memory allocated by variable number of matrices. */
{
    va_list args;
    va_start(args, count);
    while (count--) {
	SMOL_Free(va_arg(args, SMOL_Matrix*));
    }
    va_end(args);
}
