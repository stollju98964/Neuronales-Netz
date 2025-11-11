#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "neuralNetwork.h"

#define BUFFER_SIZE 100
//#define FILE_HEADER_STRING "__info2_neural_network_file_format__"

static void softmax(Matrix *matrix)
{
    if(matrix->cols > 0)
    {
        double *colSums = (double *)calloc(matrix->cols, sizeof(double));

        if(colSums != NULL)
        {
            for(int colIdx = 0; colIdx < matrix->cols; colIdx++)
            {
                for(int rowIdx = 0; rowIdx < matrix->rows; rowIdx++)
                {
                    MatrixType expValue = exp(getMatrixAt(*matrix, rowIdx, colIdx));
                    setMatrixAt(expValue, *matrix, rowIdx, colIdx);
                    colSums[colIdx] += expValue;
                }
            }

            for(int colIdx = 0; colIdx < matrix->cols; colIdx++)
            {
                for(int rowIdx = 0; rowIdx < matrix->rows; rowIdx++)
                {
                    MatrixType normalizedValue = getMatrixAt(*matrix, rowIdx, colIdx) / colSums[colIdx];
                    setMatrixAt(normalizedValue, *matrix, rowIdx, colIdx);
                }
            }
            free(colSums);
        }
    }
}

static void relu(Matrix *matrix)
{
    for(int i = 0; i < matrix->rows * matrix->cols; i++)
    {
        matrix->buffer[i] = matrix->buffer[i] >= 0 ? matrix->buffer[i] : 0;
    }
}

static int checkFileHeader(FILE *file)
{
    int isValid = 0;
    int fileHeaderLen = strlen(FILE_HEADER_STRING);
    char buffer[BUFFER_SIZE] = {0};

    if(BUFFER_SIZE-1 < fileHeaderLen)
        fileHeaderLen = BUFFER_SIZE-1;

    if(fread(buffer, sizeof(char), fileHeaderLen, file) == fileHeaderLen)
        isValid = strcmp(buffer, FILE_HEADER_STRING) == 0;

    return isValid;
}

static unsigned int readDimension(FILE *file)
{
    int dimension = 0;

    if(fread(&dimension, sizeof(int), 1, file) != 1)
        dimension = 0;
    
    return dimension;
}

static Matrix readMatrix(FILE *file, unsigned int rows, unsigned int cols)
{
    Matrix matrix = createMatrix(rows, cols);
    
    if(matrix.buffer != NULL)
    {
        if(fread(matrix.buffer, sizeof(MatrixType), rows*cols, file) != rows*cols)
            clearMatrix(&matrix);
    }

    return matrix;
}

static Layer readLayer(FILE *file, unsigned int inputDimension, unsigned int outputDimension)
{
    Layer layer;
    layer.weights = readMatrix(file, outputDimension, inputDimension);
    layer.biases = readMatrix(file, outputDimension, 1);

    return layer;
}

static int isEmptyLayer(const Layer layer)
{
    return layer.biases.cols == 0 || layer.biases.rows == 0 || layer.biases.buffer == NULL || layer.weights.rows == 0 || layer.weights.cols == 0 || layer.weights.buffer == NULL;
}

static void clearLayer(Layer *layer)
{
    if(layer != NULL)
    {
        clearMatrix(&layer->weights);
        clearMatrix(&layer->biases);
        layer->activation = NULL;
    }
}

static void assignActivations(NeuralNetwork model)
{
    for(int i = 0; i < (int)model.numberOfLayers-1; i++)
    {
        model.layers[i].activation = relu;
    }

    if(model.numberOfLayers > 0)
        model.layers[model.numberOfLayers-1].activation = softmax;
}

NeuralNetwork loadModel(const char *path)
{
    NeuralNetwork model = {NULL, 0};
    FILE *file = fopen(path, "rb");

    if(file != NULL)
    {
        if(checkFileHeader(file))
        {
            unsigned int inputDimension = readDimension(file); 
            unsigned int outputDimension = readDimension(file);

            while(inputDimension > 0 && outputDimension > 0)
            {
                Layer layer = readLayer(file, inputDimension, outputDimension);
                Layer *layerBuffer = NULL;

                if(isEmptyLayer(layer))
                {
                    clearLayer(&layer);
                    clearModel(&model);
                    break;
                }
                
                layerBuffer = (Layer *)realloc(model.layers, (model.numberOfLayers + 1) * sizeof(Layer));

                if(layerBuffer != NULL)
                    model.layers = layerBuffer;
                else
                {
                    clearModel(&model);
                    break;
                }

                model.layers[model.numberOfLayers] = layer;
                model.numberOfLayers++;

                inputDimension = outputDimension;
                outputDimension = readDimension(file);
            }
        }
        fclose(file);

        assignActivations(model);
    }

    return model;
}

static Matrix imageBatchToMatrixOfImageVectors(const GrayScaleImage images[], unsigned int count)
{
    Matrix matrix = {0, 0, NULL};

    if(count > 0 && images != NULL)
    {
        matrix = createMatrix(images[0].height * images[0].width, count);

        if(matrix.buffer != NULL)
        {
            for(int i = 0; i < count; i++)
            {
                for(int j = 0; j < images[i].width * images[i].height; j++)
                {
                    setMatrixAt((MatrixType)images[i].buffer[j], matrix, j, i);
                }
            }
        }
    }

    return matrix;
}

static Matrix forward(const NeuralNetwork model, Matrix inputBatch)
{
    Matrix result = inputBatch;

    if(result.buffer != NULL)
    {
        for(int i = 0; i < model.numberOfLayers; i++)
        {
            Matrix biasResult;
            Matrix weightResult;
            
            weightResult = multiply(model.layers[i].weights, result);
            clearMatrix(&result);
            biasResult = add(model.layers[i].biases, weightResult);
            clearMatrix(&weightResult);

            if(model.layers[i].activation != NULL)
                model.layers[i].activation(&biasResult);
            result = biasResult;
        }
    }

    return result;
}

unsigned char *argmax(const Matrix matrix)
{
    unsigned char *maxIdx = NULL;

    if(matrix.rows > 0 && matrix.cols > 0)
    {
        maxIdx = (unsigned char *)malloc(sizeof(unsigned char) * matrix.cols);

        if(maxIdx != NULL)
        {
            for(int colIdx = 0; colIdx < matrix.cols; colIdx++)
            {
                maxIdx[colIdx] = 0;

                for(int rowIdx = 1; rowIdx < matrix.rows; rowIdx++)
                {
                    if(getMatrixAt(matrix, rowIdx, colIdx) > getMatrixAt(matrix, maxIdx[colIdx], colIdx))
                        maxIdx[colIdx] = rowIdx;
                }
            }
        }
    }

    return maxIdx;
}

unsigned char *predict(const NeuralNetwork model, const GrayScaleImage images[], unsigned int numberOfImages)
{
    Matrix inputBatch = imageBatchToMatrixOfImageVectors(images, numberOfImages);
    Matrix outputBatch = forward(model, inputBatch);

    unsigned char *result = argmax(outputBatch);
    
    clearMatrix(&outputBatch);
    
    return result;
}

void clearModel(NeuralNetwork *model)
{
    if(model != NULL)
    {
        for(int i = 0; i < model->numberOfLayers; i++)
        {
            clearLayer(&model->layers[i]);
        }
        model->layers = NULL;
        model->numberOfLayers = 0;
    }
}