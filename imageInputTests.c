
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "imageInput.h"


static void prepareImageFile(const char *path, unsigned short int width, unsigned short int height, unsigned int short numberOfImages, unsigned char label)
{
    FILE *file = fopen(path, "wb");

    if(file != NULL)
    {
        const char *fileTag = "__info2_image_file_format__";
        GrayScalePixelType *zeroBuffer = (GrayScalePixelType *)calloc(numberOfImages * width * height, sizeof(GrayScalePixelType));

        if(zeroBuffer != NULL)
        {
            fwrite(fileTag, sizeof(fileTag[0]), strlen(fileTag), file);
            fwrite(&numberOfImages, sizeof(numberOfImages), 1, file);
            fwrite(&width, sizeof(width), 1, file);
            fwrite(&height, sizeof(height), 1, file);

            for(int i = 0; i < numberOfImages; i++)
            {
                fwrite(zeroBuffer, sizeof(GrayScalePixelType), width * height, file);
                fwrite(&label, sizeof(unsigned char), 1, file);
            }

            free(zeroBuffer);
        }

        fclose(file);
    }
}


void test_readImagesReturnsCorrectNumberOfImages(void)
{
    GrayScaleImageSeries *series = NULL;
    const unsigned short expectedNumberOfImages = 2;
    const char *path = "testFile.info2";
    prepareImageFile(path, 8, 8, expectedNumberOfImages, 1);
    series = readImages(path);
    TEST_ASSERT_NOT_NULL(series);
    TEST_ASSERT_EQUAL_UINT16(expectedNumberOfImages, series->count);
    clearSeries(series);
    remove(path);
}

void test_readImagesReturnsCorrectImageWidth(void)
{
    GrayScaleImageSeries *series = NULL;
    const unsigned short expectedWidth = 10;
    const char *path = "testFile.info2";
    prepareImageFile(path, expectedWidth, 8, 2, 1);
    series = readImages(path);
    TEST_ASSERT_NOT_NULL(series);
    TEST_ASSERT_NOT_NULL(series->images);
    TEST_ASSERT_EQUAL_UINT16(2, series->count);
    TEST_ASSERT_EQUAL_UINT16(expectedWidth, series->images[0].width);
    TEST_ASSERT_EQUAL_UINT16(expectedWidth, series->images[1].width);
    clearSeries(series);
    remove(path);
}

void test_readImagesReturnsCorrectImageHeight(void)
{
    GrayScaleImageSeries *series = NULL;
    const unsigned short expectedHeight = 10;
    const char *path = "testFile.info2";
    prepareImageFile(path, 8, expectedHeight, 2, 1);
    series = readImages(path);
    TEST_ASSERT_NOT_NULL(series);
    TEST_ASSERT_NOT_NULL(series->images);
    TEST_ASSERT_EQUAL_UINT16(2, series->count);
    TEST_ASSERT_EQUAL_UINT16(expectedHeight, series->images[0].height);
    TEST_ASSERT_EQUAL_UINT16(expectedHeight, series->images[1].height);
    clearSeries(series);
    remove(path);
}

void test_readImagesReturnsCorrectLabels(void)
{
    const unsigned char expectedLabel = 15;

    GrayScaleImageSeries *series = NULL;
    const char *path = "testFile.info2";
    prepareImageFile(path, 8, 8, 2, expectedLabel);
    series = readImages(path);
    TEST_ASSERT_NOT_NULL(series);
    TEST_ASSERT_NOT_NULL(series->labels);
    TEST_ASSERT_EQUAL_UINT16(2, series->count);
    for (int i = 0; i < 2; i++) {
        TEST_ASSERT_EQUAL_UINT8(expectedLabel, series->labels[i]);
    }
    clearSeries(series);
    remove(path);
}

void test_readImagesReturnsNullOnNotExistingPath(void)
{
    const char *path = "testFile.txt";
    remove(path);
    TEST_ASSERT_NULL(readImages(path));
}

void test_readImagesFailsOnWrongFileTag(void)
{
    const char *path = "testFile.info2";
    FILE *file = fopen(path, "w");
    if(file != NULL)
    {
        fprintf(file, "some_tag                  ");
        fclose(file);
        TEST_ASSERT_NULL(readImages(path));
    }
    remove(path);
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
    
    printf("\n============================\nImage input tests\n============================\n");
    RUN_TEST(test_readImagesReturnsCorrectNumberOfImages);
    RUN_TEST(test_readImagesReturnsCorrectImageWidth);
    RUN_TEST(test_readImagesReturnsCorrectImageHeight);
    RUN_TEST(test_readImagesReturnsCorrectLabels);
    RUN_TEST(test_readImagesReturnsNullOnNotExistingPath);
    RUN_TEST(test_readImagesFailsOnWrongFileTag);

    return UNITY_END();
}