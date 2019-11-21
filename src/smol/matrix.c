#include "matrix.h"
#include <string.h>
#include <time.h>
#include <math.h>

static long _randseed = 0; // RNG Seed, 0 if unitialized. 

SMOL_Matrix SMOL_AllocMatrix(size_t nRows, size_t nCols)
/* Returns a newly allocated Matrix of given size.
 * ! Allocated memory has to be freed at some point.*/
{
    return (SMOL_Matrix){.nRows=nRows, .nCols=nCols,
			     .fields=malloc(sizeof(double)*nRows*nCols)};
}

SMOL_Matrix SMOL_RandomMatrix(size_t nRows, size_t nCols)
/* Creates a matrix of given size and fills it with random values. */
{
    if (_randseed == 0) { // Init seed
	_randseed = time(NULL)/2;
	srand(_randseed);
    }
    
    SMOL_Matrix m = SMOL_AllocMatrix(nRows, nCols);
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
	m.fields[i*size+i] = 1.0;
    
    return m;
}

SMOL_Matrix SMOL_PerspectiveMatrix(double fov, double ratio, double near, double far)
/* Construct the perspective matrix from the given parameters. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(4, 4);
    SMOL_Fill(&m, 0.0);
    
    const double t = near * tan((fov/2)*M_PI/180);
    const double r = t * ratio;
    
    SMOL_SetField(&m, 0, 0, near/r);
    SMOL_SetField(&m, 1, 1, near/t);
    SMOL_SetField(&m, 2, 2, (near-far)/(far-near));
    SMOL_SetField(&m, 2, 3, (-2*near*far)/(far-near));
    SMOL_SetField(&m, 3, 2, -1.0);
    
    return m;
}

SMOL_Matrix SMOL_CameraMatrix(const double* vec3eye, const double* vec3front, const double* vec3up)
/* Construct the camera matrix from the given vector arrays. */
{
    SMOL_Matrix transl = SMOL_EyeMatrix(4);
    for(int i = 0; i < 3; i++)
	SMOL_SetField(&transl, i, 3, -vec3eye[i]);

    SMOL_Matrix front = SMOL_CopyMat(&(SMOL_Matrix){.fields=(double*)vec3front, .nRows=3, .nCols=1});
    SMOL_Matrix right = SMOL_CrossVec(&front,
				      &(SMOL_Matrix){.fields=(double*)vec3up, .nRows=3, .nCols=1});
    SMOL_Matrix up = SMOL_CrossVec(&right, &front);

    SMOL_Matrix cam = SMOL_AllocMatrix(4, 4);
    SMOL_Fill(&cam, 0.0);
    
    SMOL_Scale(&front, -1.0);    
    SMOL_Normalize(&up);
    SMOL_Normalize(&front);
    SMOL_Normalize(&right);

    SMOL_SetColumn(&cam, 0, &right);
    SMOL_SetColumn(&cam, 1, &up);
    SMOL_SetColumn(&cam, 2, &front);
    cam = SMOL_MultiplyMat(&cam, &transl);

    SMOL_FreeV(4, &transl, &front, &right, &up);
    
    return cam;
}

SMOL_Matrix SMOL_CopyMat(const SMOL_Matrix *mat)
/* Returns a deep copy of given matrix. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(mat->nRows, mat->nCols);
    memcpy(m.fields, mat->fields, sizeof(double)*m.nRows*m.nCols);
    return m;
}

SMOL_Matrix SMOL_TransposeMat(const SMOL_Matrix *mat) 
/* Return the transposed matrix of the given matrix; A = A^T. */
{
    SMOL_Matrix m = SMOL_AllocMatrix(mat->nCols, mat->nRows);
    for (unsigned int r = 0; r < m.nRows; r++) {
	for (unsigned int c = 0; c < m.nCols; c++)
	    m.fields[r*m.nCols+c] = mat->fields[c*mat->nCols+r];
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

SMOL_Matrix SMOL_CrossVec(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB)
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

double SMOL_DotVec(const SMOL_Matrix *vecA, const SMOL_Matrix *vecB)
/* Return the dot product between vector A and B. */
{
    double d = 0.0;
    for (size_t i = 0; i < vecA->nCols*vecA->nRows; i++)
	    d += vecA->fields[i] * vecB->fields[i];

    return d;
}

double SMOL_LengthVec(const SMOL_Matrix *vec)
/* Return the length of the given vector. */
{
    return (sqrt(SMOL_DotVec(vec, vec)));
}

double SMOL_LentghSqVec(const SMOL_Matrix *vec)
/* Return the length of the vector squared. */
{
    return SMOL_DotVec(vec, vec);
}

void SMOL_Normalize(SMOL_Matrix *vec)
/* Normalize the given vector. */
{
    double l = SMOL_LengthVec(vec);
    for (size_t i = 0; i < vec->nCols*vec->nRows; i++)
	vec->fields[i] /= l;
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

int SMOL_TypeOf(const SMOL_Matrix *mat)
/* Return the SMOL_TYPE of the given matrix. */
{
    int type = 0;
    
    if (mat->fields == NULL)
	type = SMOL_TYPE_NULL;
    else if (mat->nCols == 1 && mat->nRows == 1)
	type = SMOL_TYPE_SCALAR;
    else if (mat->nCols > 1 && mat->nCols > 1)
	type = SMOL_TYPE_MATRIX;
    else if (mat->nCols == 1 && mat->nRows > 1)
	type = SMOL_TYPE_COLUMN_VECTOR;
    else if (mat->nRows == 1 && mat->nCols > 1)
	type = SMOL_TYPE_ROW_VECTOR;

    return type;
}

void SMOL_SetField(SMOL_Matrix* mat, size_t row, size_t col, double value)
/* Sets the value of given matrix at position [row, col]. */
{
    mat->fields[row*mat->nCols + col] = value;
}

void SMOL_SetRow(SMOL_Matrix *mat, size_t row, const SMOL_Matrix *vec)
/* Set the given row of the matrix to the given vector. */
{
    memcpy(&mat->fields[row*mat->nCols], vec->fields, sizeof(double) * vec->nCols*vec->nRows);
}

void SMOL_SetColumn(SMOL_Matrix *mat, size_t col, const SMOL_Matrix *vec)
/* Set the given column of the matrix to the given vector. */
{
    for(size_t i = 0; i < vec->nCols*vec->nRows; i++)
	mat->fields[i*mat->nCols+col] = vec->fields[i];
}

double SMOL_GetField(const SMOL_Matrix* mat, size_t row, size_t col)
/* Returns the value of the matrix entry at [row, col]. */
{
    return mat->fields[row*mat->nCols + col];
}

SMOL_Matrix SMOL_GetRow(const SMOL_Matrix *mat, size_t row)
/* Get the row vector of the given matrix and row. */
{
    SMOL_Matrix v = SMOL_NULLMATRIX;
    if (row > mat->nRows)
	return v;

    v = SMOL_AllocMatrix(1, mat->nCols);
    memcpy(v.fields, &mat->fields[row*mat->nCols], sizeof(double)*mat->nCols);

    return v;
}

SMOL_Matrix SMOL_GetColumn(const SMOL_Matrix *mat, size_t col)
/* Get the column vector of the given matrix and column. */
{
    SMOL_Matrix v = SMOL_NULLMATRIX;
    if (col > mat->nCols)
	return v;

    v = SMOL_AllocMatrix(mat->nRows, 1);
    for(size_t i = 0; i < mat->nRows; i++)
	v.fields[i] = mat->fields[i*mat->nCols+col];
    
    return v;
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
