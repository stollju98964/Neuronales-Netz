#include <stdlib.h>
#include <string.h>
#include "matrix.h"

// TODO Matrix-Funktionen implementieren

Matrix createMatrix(unsigned int rows, unsigned int cols)
{
    Matrix m;
    m.rows = 0;
    m.cols = 0;
    m.data = NULL;

    if (rows == 0 || cols == 0) {
        return m;
    }

    /* calloc initialisiert mit 0 (praktisch UNDEFINED_MATRIX_VALUE) */
    m.data = (MatrixType *)calloc((size_t)rows * (size_t)cols, sizeof(MatrixType));
    if (m.data == NULL) {
        /* Allocation failed -> return empty matrix */
        return m;
    }

    m.rows = rows;
    m.cols = cols;
    return m;
}

void clearMatrix(Matrix *matrix)
{
    if (matrix == NULL) return;
    if (matrix->data != NULL) {
        free(matrix->data);
        matrix->data = NULL;
    }
    matrix->rows = 0;
    matrix->cols = 0;
}

void setMatrixAt(MatrixType value, Matrix matrix, unsigned int rowIdx, unsigned int colIdx)
{
    if (matrix.data == NULL) return;
    if (rowIdx >= matrix.rows || colIdx >= matrix.cols) return;
    size_t idx = (size_t)rowIdx * matrix.cols + colIdx;
    matrix.data[idx] = value;
}

MatrixType getMatrixAt(const Matrix matrix, unsigned int rowIdx, unsigned int colIdx)
{
    if (matrix.data == NULL) return (MatrixType)UNDEFINED_MATRIX_VALUE;
    if (rowIdx >= matrix.rows || colIdx >= matrix.cols) return (MatrixType)UNDEFINED_MATRIX_VALUE;
    size_t idx = (size_t)rowIdx * matrix.cols + colIdx;
    return matrix.data[idx];
}

Matrix add(const Matrix matrix1, const Matrix matrix2)
{
    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        return createMatrix(0, 0);
    }

    Matrix result = createMatrix(matrix1.rows, matrix1.cols);
    if (result.data == NULL) return result;

    size_t total = (size_t)matrix1.rows * matrix1.cols;
    for (size_t i = 0; i < total; ++i) {
        result.data[i] = matrix1.data[i] + matrix2.data[i];
    }

    return result;
}

Matrix multiply(const Matrix matrix1, const Matrix matrix2)
{
    if (matrix1.cols != matrix2.rows) {
        return createMatrix(0, 0);
    }

    Matrix result = createMatrix(matrix1.rows, matrix2.cols);
    if (result.data == NULL) return result;

    for (unsigned int i = 0; i < matrix1.rows; ++i) {
        for (unsigned int j = 0; j < matrix2.cols; ++j) {
            MatrixType sum = 0;
            for (unsigned int k = 0; k < matrix1.cols; ++k) {
                MatrixType a = getMatrixAt(matrix1, i, k);
                MatrixType b = getMatrixAt(matrix2, k, j);
                sum += a * b;
            }
            setMatrixAt(sum, result, i, j);
        }
    }

    return result;
}