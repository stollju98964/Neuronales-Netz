#ifndef IMAGEINPUT_H
#define IMAGEINPUT_H

typedef unsigned char GrayScalePixelType;

typedef struct
{
    GrayScalePixelType *buffer;
    unsigned int width;
    unsigned int height;
} GrayScaleImage;

typedef struct
{
    GrayScaleImage *images;
    unsigned char *labels;
    unsigned int count;
} GrayScaleImageSeries;

GrayScaleImageSeries *readImages(const char *path);
void clearSeries(GrayScaleImageSeries *series);

#endif
