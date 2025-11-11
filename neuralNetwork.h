#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#define FILE_HEADER_STRING "__info2_neural_network_file_format__"


#include "imageInput.h"
#include "matrix.h"

typedef void (*ActivationFunctionType)(Matrix *X);

typedef struct
{
    Matrix weights;
    Matrix biases;
    ActivationFunctionType activation;
} Layer;

typedef struct
{
    Layer *layers;
    unsigned int numberOfLayers;
} NeuralNetwork;

NeuralNetwork loadModel(const char *path);
unsigned char *predict(const NeuralNetwork model, const GrayScaleImage images[], unsigned int numberOfImages);
void clearModel(NeuralNetwork *model);

#endif
