# Simple Matrix Operation Library

SMOL, as the name suggests, is a small C library for relatively simple matrix operations on rather small matrix objects. 

SMOL consists of two primary components;
* a basic data structure, i.e. the *SMOL_Matrix*,
* and the operations on the data structure, e.g. *SMOL_EyeNxNf()*.

Most notes in this document are not relevant for the user, but merely serve as a mental note and guidance for myself.
The use and development of this library is mainly for educational purposes. There are several, more sophisticated libraries for intensive linear algebra computations, like BLAS and LAPACK.

## The Data Structure
SMOL operates on what is essentially a pointer to a contigous block of (one-dimensional) memory, which is interpreted as a matrix in row-major order.
The interpretation requires usually two further attributes - if no context is given -, which are the number of rows and the number of columns. 
An entry i,j in the memory array is therefore indexed by [i*columns + j].

The *SMOL_Matrix* is a struct, which encapsulates the array and the size attribute into a single type. This type can be passed around, copied and assigned like any other first-class type.
Have in mind that the internal matrix array is stored as a pointer. This leads to only the pointer being copied on assignment, rather than the memory block the pointer points to. 
This effect has to be taken into account, if a deep copy is desired.

## The Operations
Operations are categorised into flavors of *Mode-of-Operation* and *Context-Orientation*. 
The two basic *Mode-of-Operation*'s are *Creation* and *Configuration*. 
A *Strong Creation* function will usually return a *SMOL_Matrix* object with a newly allocated storage space for the matrix array. 
This memory has to be freed manually afterwards, as to not cause memory leaks.
*Weak Creation* functions return a basic data type as a result of a computation, such as the dot product, or length.
Here, memory does not need to be freed and these types do not need further concern.
*Configuration* routines take a reference to an object and perform a computation on it, returning the result in the reference object. 
The reference object is usually the first parameter in the function call.
The order of parametrization is given as follows; (Output Reference Object +, Other Reference Objects *, Context Informations *).
*Context-Orientation* describes the type and structure of parameters, necessary for the correct interpretation. Most of the time, this includes the size attribute of a given matrix object.
*Context-Orientation* is provided in the form of prefixes and suffixes on the function name. Some of the more common suffixes are:
* NxNf - Function takes an array of doubles and a size. Array is interpreted to be square sized matrix.
* NxMf - Function takes an array of doubles, as well as the number of columns and rows of the matrix.
* 4x4f - Function takes an array of doubles, interpretating it as a array of a fixed size (4x4 in this case).
* Mat - Array takes a SMOL_Matrix object. No further context information is needed in most cases.
The function *SMOL_VectorDot1xNf* is an example of a *Weak Creation* with strong *Context-Orientaion*. The result of the operation is returned as a basic data type, while context is given through a prefix and a suffix.
The suffix indicates, that the function expects an array object and a single size for the object, interpreting the array as a 1xN Matrix. The (redundant) Vector prefix states, that the operation is specially defined for 
vectors and other implications - such as the difference in nTp and pnT - need not be concerned, as it is clear, that the operation is performed on vectors and returns a scalar.

