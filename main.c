#include <stdio.h>
#include <stdlib.h>
#include "imageInput.h"
#include "mnistVisualization.h"
#include "neuralNetwork.h"

int main(int argc, char *argv[])
{
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    int exitCode = EXIT_FAILURE;

    if(argc == 3)
    {
        const char *pathToMnist = argv[1];
        const char *pathToModel = argv[2];
        GrayScaleImageSeries *series = readImages(pathToMnist);

        if(series != NULL)
        {
            NeuralNetwork model = {NULL, 0};
            printf("Loaded %u images ... \n", series->count);

            model = loadModel(pathToModel);

            if(model.numberOfLayers > 0)
            {
                unsigned char *predictions = NULL;
                
                printf("Processing %u images ...\n", series->count);

                predictions = predict(model, series->images, series->count);

                if(predictions != NULL)
                {
                    showMnist(windowWidth, windowHeight, series, predictions);
                    free(predictions);
                    exitCode = EXIT_SUCCESS;
                }
                else
                {
                    fprintf(stderr, "Error while processing images ...\n");
                    exitCode = EXIT_FAILURE;
                }
                clearModel(&model);
            }
            else
            {
                fprintf(stderr, "Error while loading model from %s ...\n", pathToModel);
                exitCode = EXIT_FAILURE;
            }
        }
        else
        {
            fprintf(stderr, "Error while reading images from %s ...\n", pathToMnist);
            exitCode = EXIT_FAILURE;
        }
        clearSeries(series);
    }
    else
    {
        fprintf(stderr, "Usage: %s <mnist file> <model file>\n", argv[0]);
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}