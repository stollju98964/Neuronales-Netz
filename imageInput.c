#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageInput.h"

#define BUFFER_SIZE 100
#define FILE_HEADER_STRING "__info2_image_file_format__"

// TODO Implementieren Sie geeignete Hilfsfunktionen für das Lesen der Bildserie aus einer Datei

static int read_file_tag(FILE *f)
{
    size_t tag_len = strlen(FILE_HEADER_STRING);
    char buf[BUFFER_SIZE];
    if (tag_len + 1 > sizeof(buf)) return 0; /* safety */
    if (fread(buf, 1, tag_len, f) != tag_len) return 0;
    buf[tag_len] = '\0';
    return (strncmp(buf, FILE_HEADER_STRING, tag_len) == 0);
}

static int read_u16(FILE *f, unsigned short *out)
{
    if (fread(out, sizeof(unsigned short), 1, f) != 1) return 0;
    return 1;
}

static int read_image_data(FILE *f, GrayScalePixelType *buffer, unsigned int pixels)
{
    if (fread(buffer, sizeof(GrayScalePixelType), pixels, f) != pixels) return 0;
    return 1;
}

// TODO Vervollständigen Sie die Funktion readImages unter Benutzung Ihrer Hilfsfunktionen
GrayScaleImageSeries *readImages(const char *path)
{
    if (path == NULL) return NULL;

    FILE *file = fopen(path, "rb");
    if (file == NULL) return NULL;

    if (!read_file_tag(file)) {
        fclose(file);
        return NULL;
    }

    unsigned short count16 = 0;
    unsigned short width = 0;
    unsigned short height = 0;

    if (!read_u16(file, &count16) || !read_u16(file, &width) || !read_u16(file, &height)) {
        fclose(file);
        return NULL;
    }

    unsigned int count = (unsigned int)count16;
    if (count == 0) {
        fclose(file);
        return NULL;
    }

    GrayScaleImageSeries *series = (GrayScaleImageSeries *)malloc(sizeof(GrayScaleImageSeries));
    if (series == NULL) {
        fclose(file);
        return NULL;
    }

    series->count = count;
    series->images = (GrayScaleImage *)calloc(count, sizeof(GrayScaleImage));
    series->labels = (unsigned char *)calloc(count, sizeof(unsigned char));
    if (series->images == NULL || series->labels == NULL) {
        clearSeries(series);
        fclose(file);
        return NULL;
    }

    unsigned int pixels = (unsigned int)width * (unsigned int)height;
    for (unsigned int i = 0; i < count; ++i) {
        GrayScalePixelType *imgbuf = (GrayScalePixelType *)malloc((size_t)pixels * sizeof(GrayScalePixelType));
        if (imgbuf == NULL) {
            clearSeries(series);
            fclose(file);
            return NULL;
        }

        if (!read_image_data(file, imgbuf, pixels)) {
            free(imgbuf);
            clearSeries(series);
            fclose(file);
            return NULL;
        }

        unsigned char label = 0;
        if (fread(&label, sizeof(unsigned char), 1, file) != 1) {
            free(imgbuf);
            clearSeries(series);
            fclose(file);
            return NULL;
        }

        series->images[i].buffer = imgbuf;
        series->images[i].width = width;
        series->images[i].height = height;
        series->labels[i] = label;
    }

    fclose(file);
    return series;
}

// TODO Vervollständigen Sie die Funktion clearSeries, welche eine Bildserie vollständig aus dem Speicher freigibt
void clearSeries(GrayScaleImageSeries *series)
{
    if (series == NULL) return;

    if (series->images != NULL) {
        for (unsigned int i = 0; i < series->count; ++i) {
            if (series->images[i].buffer != NULL) {
                free(series->images[i].buffer);
                series->images[i].buffer = NULL;
            }
        }
        free(series->images);
        series->images = NULL;
    }

    if (series->labels != NULL) {
        free(series->labels);
        series->labels = NULL;
    }

    free(series);
}