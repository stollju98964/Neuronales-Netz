#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "unity.h"
#include "neuralNetwork.h"


static void prepareNeuralNetworkFile(const char *path, const NeuralNetwork nn)
{
    // TODO
}

void test_loadModelReturnsCorrectNumberOfLayers(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType buffer1[] = {1, 2, 3, 4, 5, 6};
    MatrixType buffer2[] = {1, 2, 3, 4, 5, 6};
    Matrix weights1 = {.buffer=buffer1, .rows=3, .cols=2};
    Matrix weights2 = {.buffer=buffer2, .rows=2, .cols=3};
    MatrixType buffer3[] = {1, 2, 3};
    MatrixType buffer4[] = {1, 2};
    Matrix biases1 = {.buffer=buffer3, .rows=3, .cols=1};
    Matrix biases2 = {.buffer=buffer4, .rows=2, .cols=1};
    Layer layers[] = {{.weights=weights1, .biases=biases1}, {.weights=weights2, .biases=biases2}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=2};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_EQUAL_INT(expectedNet.numberOfLayers, netUnderTest.numberOfLayers);
    clearModel(&netUnderTest);
}

void test_loadModelReturnsCorrectWeightDimensions(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType weightBuffer[] = {1, 2, 3, 4, 5, 6};
    Matrix weights = {.buffer=weightBuffer, .rows=3, .cols=2};
    MatrixType biasBuffer[] = {7, 8, 9};
    Matrix biases = {.buffer=biasBuffer, .rows=3, .cols=1};
    Layer layers[] = {{.weights=weights, .biases=biases}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=1};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_TRUE(netUnderTest.numberOfLayers > 0);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.rows, netUnderTest.layers[0].weights.rows);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.cols, netUnderTest.layers[0].weights.cols);
    clearModel(&netUnderTest);
}

void test_loadModelReturnsCorrectBiasDimensions(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType weightBuffer[] = {1, 2, 3, 4, 5, 6};
    Matrix weights = {.buffer=weightBuffer, .rows=3, .cols=2};
    MatrixType biasBuffer[] = {7, 8, 9};
    Matrix biases = {.buffer=biasBuffer, .rows=3, .cols=1};
    Layer layers[] = {{.weights=weights, .biases=biases}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=1};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_TRUE(netUnderTest.numberOfLayers > 0);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].biases.rows, netUnderTest.layers[0].biases.rows);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].biases.cols, netUnderTest.layers[0].biases.cols);
    clearModel(&netUnderTest);
}

void test_loadModelReturnsCorrectWeights(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType weightBuffer[] = {1, 2, 3, 4, 5, 6};
    Matrix weights = {.buffer=weightBuffer, .rows=3, .cols=2};
    MatrixType biasBuffer[] = {7, 8, 9};
    Matrix biases = {.buffer=biasBuffer, .rows=3, .cols=1};
    Layer layers[] = {{.weights=weights, .biases=biases}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=1};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_TRUE(netUnderTest.numberOfLayers > 0);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.rows, netUnderTest.layers[0].weights.rows);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.cols, netUnderTest.layers[0].weights.cols);
    int n = netUnderTest.layers[0].weights.rows * netUnderTest.layers[0].weights.cols;
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedNet.layers[0].weights.buffer, netUnderTest.layers[0].weights.buffer, n);
    clearModel(&netUnderTest);
}

void test_loadModelReturnsCorrectBiases(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType weightBuffer[] = {1, 2, 3, 4, 5, 6};
    Matrix weights = {.buffer=weightBuffer, .rows=3, .cols=2};
    MatrixType biasBuffer[] = {7, 8, 9};
    Matrix biases = {.buffer=biasBuffer, .rows=3, .cols=1};
    Layer layers[] = {{.weights=weights, .biases=biases}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=1};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_TRUE(netUnderTest.numberOfLayers > 0);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.rows, netUnderTest.layers[0].weights.rows);
    TEST_ASSERT_EQUAL_INT(expectedNet.layers[0].weights.cols, netUnderTest.layers[0].weights.cols);
    int n = netUnderTest.layers[0].biases.rows * netUnderTest.layers[0].biases.cols;
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedNet.layers[0].biases.buffer, netUnderTest.layers[0].biases.buffer, n);
    clearModel(&netUnderTest);
}

void test_loadModelFailsOnWrongFileTag(void)
{
    const char *path = "some_nn_test_file.info2";
    NeuralNetwork netUnderTest;
    FILE *file = fopen(path, "wb");

    if(file != NULL)
    {
        const char *fileTag = "info2_neural_network_file_format";

        fwrite(fileTag, sizeof(char), strlen(fileTag), file);

        fclose(file);
    }

    netUnderTest = loadModel(path);

    remove(path);

    TEST_ASSERT_NULL(netUnderTest.layers);
    TEST_ASSERT_EQUAL_INT(0, netUnderTest.numberOfLayers);
}

void test_clearModelSetsMembersToNull(void)
{
    const char *path = "some__nn_test_file.info2";
    MatrixType weightBuffer[] = {1, 2, 3, 4, 5, 6};
    Matrix weights = {.buffer=weightBuffer, .rows=3, .cols=2};
    MatrixType biasBuffer[] = {7, 8, 9};
    Matrix biases = {.buffer=biasBuffer, .rows=3, .cols=1};
    Layer layers[] = {{.weights=weights, .biases=biases}};

    NeuralNetwork expectedNet = {.layers=layers, .numberOfLayers=1};
    NeuralNetwork netUnderTest;

    prepareNeuralNetworkFile(path, expectedNet);

    netUnderTest = loadModel(path);
    remove(path);

    TEST_ASSERT_NOT_NULL(netUnderTest.layers);
    TEST_ASSERT_TRUE(netUnderTest.numberOfLayers > 0);
    clearModel(&netUnderTest);
    TEST_ASSERT_NULL(netUnderTest.layers);
    TEST_ASSERT_EQUAL_INT(0, netUnderTest.numberOfLayers);
}

static void someActivation(Matrix *matrix)
{
    for(int i = 0; i < matrix->rows * matrix->cols; i++)
    {
        matrix->buffer[i] = fabs(matrix->buffer[i]);
    }
}

void test_predictReturnsCorrectLabels(void)
{
    const unsigned char expectedLabels[] = {4, 2};
    GrayScalePixelType imageBuffer1[] = {10, 30, 25, 17};
    GrayScalePixelType imageBuffer2[] = {20, 40, 10, 128};
    GrayScaleImage inputImages[] = {{.buffer=imageBuffer1, .width=2, .height=2}, {.buffer=imageBuffer2, .width=2, .height=2}};
    MatrixType weightsBuffer1[] = {1, -2, 3, -4, 5, -6, 7, -8};
    MatrixType weightsBuffer2[] = {-9, 10, 11, 12, 13, 14};
    MatrixType weightsBuffer3[] = {-15, 16, 17, 18, -19, 20, 21, 22, 23, -24, 25, 26, 27, -28, -29};
    Matrix weights1 = {.buffer=weightsBuffer1, .rows=2, .cols=4};
    Matrix weights2 = {.buffer=weightsBuffer2, .rows=3, .cols=2};
    Matrix weights3 = {.buffer=weightsBuffer3, .rows=5, .cols=3};
    MatrixType biasBuffer1[] = {200, 0};
    MatrixType biasBuffer2[] = {0, -100, 0};
    MatrixType biasBuffer3[] = {0, -1000, 0, 2000, 0};
    Matrix biases1 = {.buffer=biasBuffer1, .rows=2, .cols=1};
    Matrix biases2 = {.buffer=biasBuffer2, .rows=3, .cols=1};
    Matrix biases3 = {.buffer=biasBuffer3, .rows=5, .cols=1};
    Layer layers[] = {{.weights=weights1, .biases=biases1, .activation=someActivation}, \
                      {.weights=weights2, .biases=biases2, .activation=someActivation}, \
                      {.weights=weights3, .biases=biases3, .activation=someActivation}};
    NeuralNetwork netUnderTest = {.layers=layers, .numberOfLayers=3};
    unsigned char *predictedLabels = predict(netUnderTest, inputImages, 2);
    TEST_ASSERT_NOT_NULL(predictedLabels);
    int n = (int)(sizeof(expectedLabels) / sizeof(expectedLabels[0]));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLabels, predictedLabels, n);
    free(predictedLabels);
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

    printf("\n============================\nNeural network tests\n============================\n");
    RUN_TEST(test_loadModelReturnsCorrectNumberOfLayers);
    RUN_TEST(test_loadModelReturnsCorrectWeightDimensions);
    RUN_TEST(test_loadModelReturnsCorrectBiasDimensions);
    RUN_TEST(test_loadModelReturnsCorrectWeights);
    RUN_TEST(test_loadModelReturnsCorrectBiases);
    RUN_TEST(test_loadModelFailsOnWrongFileTag);
    RUN_TEST(test_clearModelSetsMembersToNull);
    RUN_TEST(test_predictReturnsCorrectLabels);

    return UNITY_END();
}