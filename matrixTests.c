
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "unity.h"


void test_createMatrixFailsOnZeroDimensions(void)
{
    Matrix matrixToTest1 = createMatrix(0, 1);
    Matrix matrixToTest2 = createMatrix(1, 0);
    TEST_ASSERT_NULL(matrixToTest1.buffer);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest1.rows);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest1.cols);
    TEST_ASSERT_NULL(matrixToTest2.buffer);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest2.rows);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest2.cols);
}

void test_createMatrixReturnsCorrectMatrixDimensions(void)
{
    const unsigned int expectedRows = 15;
    const unsigned int expectedCols = 10;
    
    Matrix matrixToTest = createMatrix(expectedRows, expectedCols);
    TEST_ASSERT_EQUAL_UINT32(expectedRows, matrixToTest.rows);
    TEST_ASSERT_EQUAL_UINT32(expectedCols, matrixToTest.cols);
    free(matrixToTest.buffer);
}

void test_clearMatrixSetsMembersToNull(void)
{
    Matrix matrixToTest = createMatrix(10, 10);
    clearMatrix(&matrixToTest);
    TEST_ASSERT_NULL(matrixToTest.buffer);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest.rows);
    TEST_ASSERT_EQUAL_UINT32(0, matrixToTest.cols);
}

void test_addReturnsCorrectResult(void)
{
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {7, 8, 9, 10, 11, 12};
    Matrix matrix1 = {.rows=2, .cols=3, .buffer=buffer1};
    Matrix matrix2 = {.rows=2, .cols=3, .buffer=buffer2};

    Matrix result = add(matrix1, matrix2);

    float expectedResults[] = {8, 10, 12, 14, 16, 18};

    TEST_ASSERT_EQUAL_UINT32(matrix1.rows, result.rows);
    TEST_ASSERT_EQUAL_UINT32(matrix2.rows, result.rows);
    TEST_ASSERT_EQUAL_UINT32(matrix1.cols, result.cols);
    TEST_ASSERT_EQUAL_UINT32(matrix2.cols, result.cols);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedResults)/sizeof(expectedResults[0]), result.rows * result.cols);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expectedResults, result.buffer, result.cols * result.rows);
    free(result.buffer);
}

void test_addFailsOnDifferentInputDimensions(void)
{
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {7, 8, 9, 10, 11, 12};
    Matrix matrix1 = {.rows=2, .cols=3, .buffer=buffer1};
    Matrix matrix2 = {.rows=3, .cols=2, .buffer=buffer2};
    
    Matrix result = add(matrix1, matrix2);

    TEST_ASSERT_NULL(result.buffer);
    TEST_ASSERT_EQUAL_UINT32(0, result.rows);
    TEST_ASSERT_EQUAL_UINT32(0, result.cols);
}

void test_addSupportsBroadcasting(void)
{
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {7, 8};
    Matrix matrix1 = {.rows=2, .cols=3, .buffer=buffer1};
    Matrix matrix2 = {.rows=2, .cols=1, .buffer=buffer2};

    Matrix result1 = add(matrix1, matrix2);
    Matrix result2 = add(matrix2, matrix1);

    float expectedResults[] = {8, 9, 10, 12, 13, 14};

    TEST_ASSERT_EQUAL_UINT32(matrix1.rows, result1.rows);
    TEST_ASSERT_EQUAL_UINT32(matrix1.cols, result1.cols);
    TEST_ASSERT_EQUAL_UINT32(matrix1.rows, result2.rows);
    TEST_ASSERT_EQUAL_UINT32(matrix1.cols, result2.cols);

    TEST_ASSERT_EQUAL_INT(sizeof(expectedResults)/sizeof(expectedResults[0]), result1.rows * result1.cols);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expectedResults, result1.buffer, result1.cols * result1.rows);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedResults)/sizeof(expectedResults[0]), result2.rows * result2.cols);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expectedResults, result2.buffer, result2.cols * result2.rows);

    free(result1.buffer);
    free(result2.buffer);
}

void test_multiplyReturnsCorrectResults(void)
{
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    Matrix matrix1 = {.rows=2, .cols=3, .buffer=buffer1};
    Matrix matrix2 = {.rows=3, .cols=4, .buffer=buffer2};
    
    Matrix result = multiply(matrix1, matrix2);

    float expectedResults[] = {74, 80, 86, 92, 173, 188, 203, 218};

    TEST_ASSERT_EQUAL_UINT32(matrix1.rows, result.rows);
    TEST_ASSERT_EQUAL_UINT32(matrix2.cols, result.cols);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedResults)/sizeof(expectedResults[0]), result.rows * result.cols);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expectedResults, result.buffer, result.cols * result.rows);

    free(result.buffer);
}

void test_multiplyFailsOnWrongInputDimensions(void)
{
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {7, 8, 9, 10, 11, 12};
    Matrix matrix1 = {.rows=2, .cols=3, .buffer=buffer1};
    Matrix matrix2 = {.rows=2, .cols=3, .buffer=buffer2};

    Matrix result = multiply(matrix1, matrix2);

    TEST_ASSERT_NULL(result.buffer);
    TEST_ASSERT_EQUAL_UINT32(0, result.rows);
    TEST_ASSERT_EQUAL_UINT32(0, result.cols);
}

void test_getMatrixAtReturnsCorrectResult(void)
{
    MatrixType buffer[] = {1, 2, 3, 4, 5, 6};
    Matrix matrixToTest = {.rows=2, .cols=3, .buffer=buffer};
    int expectedResult = buffer[4];

    TEST_ASSERT_EQUAL_INT(expectedResult, getMatrixAt(matrixToTest, 1, 1));
}

void test_getMatrixAtFailsOnIndicesOutOfRange(void)
{
    MatrixType buffer[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Matrix matrixToTest = {.rows=2, .cols=3, .buffer=buffer};

    TEST_ASSERT_EQUAL_INT(UNDEFINED_MATRIX_VALUE, getMatrixAt(matrixToTest, 2, 3));
}

void test_setMatrixAtSetsCorrectValue(void)
{
    const int expectedResult = -1;
    MatrixType buffer[] = {1, 2, 3, 4, 5, 6};
    Matrix matrixUnderTest = {.rows=2, .cols=3, .buffer=buffer};

    setMatrixAt(expectedResult, matrixUnderTest, 1, 2);
    TEST_ASSERT_EQUAL_INT(expectedResult, buffer[5]);
}

void test_setMatrixAtFailsOnIndicesOutOfRange(void)
{
    const float expectedResults[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    MatrixType buffer[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Matrix matrixToTest = {.rows=2, .cols=3, .buffer=buffer};

    setMatrixAt(-1, matrixToTest, 2, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expectedResults, matrixToTest.buffer, matrixToTest.cols * matrixToTest.rows);
}

void setUp(void) {
    // Falls notwendig, kann hier Vorbereitungsarbeit gemacht werden
}

void tearDown(void) {
    // Hier kann Bereinigungsarbeit nach jedem Test durchgeführt werden
}

int main()
{
    UNITY_BEGIN();

    printf("============================\nMatrix tests\n============================\n");
    RUN_TEST(test_createMatrixReturnsCorrectMatrixDimensions);
    RUN_TEST(test_createMatrixFailsOnZeroDimensions);
    RUN_TEST(test_clearMatrixSetsMembersToNull);
    RUN_TEST(test_addReturnsCorrectResult);
    RUN_TEST(test_addFailsOnDifferentInputDimensions);
    RUN_TEST(test_addSupportsBroadcasting);
    RUN_TEST(test_multiplyReturnsCorrectResults);
    RUN_TEST(test_multiplyFailsOnWrongInputDimensions);
    RUN_TEST(test_getMatrixAtReturnsCorrectResult);
    RUN_TEST(test_getMatrixAtFailsOnIndicesOutOfRange);
    RUN_TEST(test_setMatrixAtSetsCorrectValue);
    RUN_TEST(test_setMatrixAtFailsOnIndicesOutOfRange);

    return UNITY_END();
}