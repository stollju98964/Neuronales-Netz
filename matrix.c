#include <stdlib.h>
#include <string.h>
#include "matrix.h"

// TODO Matrix-Funktionen implementieren

Matrix createMatrix(unsigned int rows, unsigned int cols)
{
    Matrix m;
    m.rows = 0;
    m.cols = 0;
    m.buffer = NULL;

    if (rows == 0 || cols == 0) {
        return m;
    }

    /* calloc initialisiert mit 0 (praktisch UNDEFINED_MATRIX_VALUE) */
    m.buffer = (MatrixType *)calloc((size_t)rows * (size_t)cols, sizeof(MatrixType));
    if (m.buffer == NULL) {
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
    if (matrix->buffer != NULL) {
        free(matrix->buffer);
        matrix->buffer = NULL;
    }
    matrix->rows = 0;
    matrix->cols = 0;
}

void setMatrixAt(MatrixType value, Matrix matrix, unsigned int rowIdx, unsigned int colIdx)
{
    if (matrix.buffer == NULL) return;
    if (rowIdx >= matrix.rows || colIdx >= matrix.cols) return;
    size_t idx = (size_t)rowIdx * matrix.cols + colIdx;
    matrix.buffer[idx] = value;
}

MatrixType getMatrixAt(const Matrix matrix, unsigned int rowIdx, unsigned int colIdx)
{
    if (matrix.buffer == NULL) return (MatrixType)UNDEFINED_MATRIX_VALUE;
    if (rowIdx >= matrix.rows || colIdx >= matrix.cols) return (MatrixType)UNDEFINED_MATRIX_VALUE;
    size_t idx = (size_t)rowIdx * matrix.cols + colIdx;
    return matrix.buffer[idx];
}

Matrix add(const Matrix matrix1, const Matrix matrix2)
{
    // Elementweise Addition
    if (matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols) {
        Matrix result = createMatrix(matrix1.rows, matrix1.cols);
        if (result.buffer == NULL) return result;
        size_t total = (size_t)matrix1.rows * matrix1.cols;
        for (size_t i = 0; i < total; ++i) {
            result.buffer[i] = matrix1.buffer[i] + matrix2.buffer[i];
        }
        return result;
    }

    // Broadcasting: matrix2 ist Spaltenvektor
    if (matrix1.rows == matrix2.rows && matrix2.cols == 1) {
        Matrix result = createMatrix(matrix1.rows, matrix1.cols);
        if (result.buffer == NULL) return result;
        for (unsigned int i = 0; i < matrix1.rows; ++i) {
            MatrixType v = getMatrixAt(matrix2, i, 0);
            for (unsigned int j = 0; j < matrix1.cols; ++j) {
                setMatrixAt(getMatrixAt(matrix1, i, j) + v, result, i, j);
            }
        }
        return result;
    }

    // Broadcasting: matrix1 ist Spaltenvektor
    if (matrix2.rows == matrix1.rows && matrix1.cols == 1) {
        Matrix result = createMatrix(matrix2.rows, matrix2.cols);
        if (result.buffer == NULL) return result;
        for (unsigned int i = 0; i < matrix2.rows; ++i) {
            MatrixType v = getMatrixAt(matrix1, i, 0);
            for (unsigned int j = 0; j < matrix2.cols; ++j) {
                setMatrixAt(v + getMatrixAt(matrix2, i, j), result, i, j);
            }
        }
        return result;
    }

    // Nicht unterstützte Dimensionen
    return createMatrix(0, 0);
}

Matrix multiply(const Matrix matrix1, const Matrix matrix2)
{
    if (matrix1.cols != matrix2.rows) {
        return createMatrix(0, 0);
    }

    Matrix result = createMatrix(matrix1.rows, matrix2.cols);
    if (result.buffer == NULL) return result;

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