#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mnistVisualization.h"
#include "raylib.h"

#define MAX_TEXT_LEN 100

typedef struct
{
    Vector2 position;
    const GrayScaleImageSeries *series;
    const unsigned char *predictions;
    unsigned int currentIdx;
    Vector2 pixelSize;
} MnistVisualization;

typedef struct
{
    char text[MAX_TEXT_LEN];
    Vector2 position;
    unsigned int fontSize;
    Color color;
} TextLabel;

static TextLabel *createTextLabel(const char *text, unsigned int fontSize, Color color)
{
    TextLabel *label = NULL;

    if(text != NULL)
    {
        label = (TextLabel *)malloc(sizeof(TextLabel));

        if(label != NULL)
        {
            strncpy(label->text, text, MAX_TEXT_LEN);
            label->text[MAX_TEXT_LEN-1] = '\0';
            label->position.x = 0;
            label->position.y = 0;
            label->fontSize = fontSize;
            label->color = color;
        }
    }

    return label;
}

static MnistVisualization *createVisualizationContainer(const GrayScaleImageSeries *series, const unsigned char predictions[], Vector2 size)
{
    MnistVisualization *container = NULL;
    
    if(size.x > 0 && size.y > 0 && series != NULL && series->images != NULL && series->count > 0 && predictions != NULL)
    {
        container = (MnistVisualization *)calloc(1, sizeof(MnistVisualization));

        if(container != NULL)
        {
            Vector2 pixelSize = {(int)(size.x / series->images[0].width), (int)(size.y / series->images[0].height)};
            container->pixelSize = pixelSize;
            container->series = series;
            container->predictions = predictions;
        }
    }

    return container;
}

static void drawTextLabel(const TextLabel *label)
{
    DrawText(label->text, label->position.x, label->position.y, label->fontSize, label->color);
}

static void drawDigit(const GrayScaleImage image, Vector2 position, Vector2 pixelSize)
{
    float xOffset = 0;
    float yOffset = position.y - pixelSize.y;
    Color color = {0};
    color.a = 255;

    for(int i = 0; i < image.width * image.height; i++)
    {
        if(i % image.width == 0)
        {
            xOffset = position.x;
            yOffset += pixelSize.y;
        }

        color.r = image.buffer[i];
        color.b = image.buffer[i];
        color.g = image.buffer[i];

        DrawRectangle(xOffset, yOffset, pixelSize.x, pixelSize.y, color);
        xOffset += pixelSize.x;
    }
}

static void drawAll(const MnistVisualization *container, const TextLabel *navigationLabel, const TextLabel *predictionLabel)
{
    BeginDrawing();
    ClearBackground(BLACK);
    
    drawDigit(container->series->images[container->currentIdx], container->position, container->pixelSize);
    drawTextLabel(navigationLabel);
    drawTextLabel(predictionLabel);
        
    EndDrawing();
}

static int checkUserInput()
{
    int inputResult = 0;

    if(IsKeyReleased(KEY_LEFT))
        inputResult = -1;
    else if(IsKeyReleased(KEY_RIGHT))
        inputResult = 1;
    
    return inputResult;
}

static void updateDisplayContainer(MnistVisualization *container, int updateDirection)
{
    int newIndex = (int)container->currentIdx + updateDirection;

    if(newIndex < 0)
        newIndex = 0;
    else if(newIndex >= container->series->count)
        newIndex = container->series->count - 1;

    container->currentIdx = newIndex;
}

static void updatePredictionLabel(TextLabel *predictionLabel, unsigned char trueLabel, unsigned char predictedLabel)
{
    snprintf(predictionLabel->text, MAX_TEXT_LEN, "True label:        %u\nPredicted label:  %u", trueLabel, predictedLabel);
}

static void update(MnistVisualization *container, TextLabel *predictionLabel, int updateDirection)
{
    updateDisplayContainer(container, updateDirection);
    updatePredictionLabel(predictionLabel, container->series->labels[container->currentIdx], container->predictions[container->currentIdx]);
}

void showMnist(unsigned int windowWidth, unsigned int windowHeight, const GrayScaleImageSeries *series, const unsigned char predictions[])
{
    const Vector2 windowSize = {windowWidth, windowHeight};

    MnistVisualization *container = createVisualizationContainer(series, predictions, windowSize);
    TextLabel *navigationLabel = createTextLabel("Use left and right key to navigate ...", 20, WHITE);
    TextLabel *predictionLabel = createTextLabel("", 20, WHITE);
    predictionLabel->position.x = 10;
    predictionLabel->position.y = windowSize.y - 50;

    if(container != NULL && navigationLabel != NULL && predictionLabel != NULL)
    {
        InitWindow(windowSize.x, windowSize.y, "MNIST Browser");

        SetTargetFPS(60);

        while (!WindowShouldClose())
        {
            int updateDirection = checkUserInput();
            update(container, predictionLabel, updateDirection);
            drawAll(container, navigationLabel, predictionLabel);
        }
    }

    CloseWindow();

    free(container);
    free(navigationLabel);
    free(predictionLabel);
}