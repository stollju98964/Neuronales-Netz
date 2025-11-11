#ifndef MATRIX_H
#define MATRIX_H

#define UNDEFINED_MATRIX_VALUE 0

typedef float MatrixType;

// TODO Matrixtyp definieren

typedef struct {
    unsigned int rows;
    unsigned int cols;
    MatrixType *data; /* Zeiger auf rows*cols Elemente, zeilen-major */
} Matrix;

Matrix createMatrix(unsigned int rows, unsigned int cols);
void clearMatrix(Matrix *matrix);
void setMatrixAt(MatrixType value, Matrix matrix, unsigned int rowIdx, unsigned int colIdx);
MatrixType getMatrixAt(const Matrix matrix, unsigned int rowIdx, unsigned int colIdx);
Matrix add(const Matrix matrix1, const Matrix matrix2);
Matrix multiply(const Matrix matrix1, const Matrix matrix2);
#endif
