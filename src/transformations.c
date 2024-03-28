/* Basic editing features - rotate, scale and translate, redo and undo */
/* NITHYA*/
#include <stdio.h>
#include <stdlib.h>
#include "transformations.h"
#include "parser.h"

Pixel bilinear_interpolate(double x, double y, long cols, long rows, Pixel **the_image) {
    double fraction_x, fraction_y, one_minus_x, one_minus_y;
    int ceil_x, ceil_y, floor_x, floor_y;
    Pixel result = {0};

    // Check if the coordinates are within the image bounds
    if (x < 0.0 || x >= (double)(cols - 1) || y < 0.0 || y >= (double)(rows - 1))
        return result;

    // Calculate floor and ceil coordinates
    floor_x = (int)floor(x);
    floor_y = (int)floor(y);
    ceil_x = (int)ceil(x);
    ceil_y = (int)ceil(y);

    // Calculate fractions for interpolation
    fraction_x = x - floor(x);
    fraction_y = y - floor(y);
    one_minus_x = 1.0 - fraction_x;
    one_minus_y = 1.0 - fraction_y;

    result.r = one_minus_y * (one_minus_x * the_image[floor_y][floor_x].r + fraction_x * the_image[floor_y][ceil_x].r) +
               fraction_y * (one_minus_x * the_image[ceil_y][floor_x].r + fraction_x * the_image[ceil_y][ceil_x].r);
    result.g = one_minus_y * (one_minus_x * the_image[floor_y][floor_x].g + fraction_x * the_image[floor_y][ceil_x].g) +
               fraction_y * (one_minus_x * the_image[ceil_y][floor_x].g + fraction_x * the_image[ceil_y][ceil_x].g);
    result.b = one_minus_y * (one_minus_x * the_image[floor_y][floor_x].b + fraction_x * the_image[floor_y][ceil_x].b) +
               fraction_y * (one_minus_x * the_image[ceil_y][floor_x].b + fraction_x * the_image[ceil_y][ceil_x].b);
    return result;
}

void EDIT_Rotate(const Image *image, Image *rotatedImage, float angle) {
    double radian_angle = angle / RADTOANG;
    double cosa = cos(radian_angle);
    double sina = sin(radian_angle);
    double m = image->width / 2.0;
    double n = image->height / 2.0;
    int new_i, new_j;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            // Apply rotation
            double tmpx = (double)(j) * cosa - (double)(i) * sina - (double)(m) * cosa + (double)(m) + (double)(n) * sina;
            double tmpy = (double)(i) * cosa + (double)(j) * sina - (double)(m) * sina - (double)(n) * cosa + (double)(n);

            /* Round to nearest integer to get pixel coordinates */
            new_j = (int)round(tmpx);
            new_i = (int)round(tmpy);

            // Check if the new coordinates are within the image bounds
            if (new_j < 0 || new_j >= image->width || new_i < 0 || new_i >= image->height) {
                rotatedImage->pixels[i][j].r = FILL;
                rotatedImage->pixels[i][j].g = FILL;
                rotatedImage->pixels[i][j].b = FILL;
            } else {
                rotatedImage->pixels[i][j] = image->pixels[new_i][new_j];
            }
        }
    }
}

void EDIT_Scale(const Image *image, Image *scaledImage, int scale) {
    // Calculate scaling factors for width and height
    double scaleX = (double)scale;
    double scaleY = (double)scale;

    // Calculate dimensions of the scaled image
    int scaledRows = image->height * scale;
    int scaledCols = image->width * scale;

    // Allocate memory for the scaled image
    scaledImage->pixels = (Pixel **)malloc(scaledRows * sizeof(Pixel *));
    for (int i = 0; i < scaledRows; i++) {
        scaledImage->pixels[i] = (Pixel *)malloc(scaledCols * sizeof(Pixel));
    }

    // Iterate over each pixel in the scaled image
    for (int i = 0; i < scaledRows; i++) {
        for (int j = 0; j < scaledCols; j++) {
            // Map pixel coordinates back to original image
            double pre_i = (double)i / scaleY;
            double pre_j = (double)j / scaleX;

            scaledImage->pixels[i][j] = bilinear_interpolate(pre_j, pre_i, image->width, image->height, image->pixels);
        }
    }

    scaledImage->width = scaledCols;
    scaledImage->height = scaledRows;
}

/*
int main() {
    Example image dimensions
    int rows = 14;
    int cols = 3;
    // Angle of rotation (in degrees)
    float angle = 45.0;
    // Scaling factor
    int scale = 2;

    // Allocate memory for the original and rotated images
    Image image, rotatedImage, scaledImage;
    image.width = cols;
    image.height = rows;
    image.pixels = (Pixel **)malloc(rows * sizeof(Pixel *));
    rotatedImage.width = cols;
    rotatedImage.height = rows;
    rotatedImage.pixels = (Pixel **)malloc(rows * sizeof(Pixel *));
    scaledImage.width = cols * scale;
    scaledImage.height = rows * scale;
    scaledImage.pixels = (Pixel **)malloc((rows * scale) * sizeof(Pixel *));

    for (int i = 0; i < rows; i++) {
        image.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
        rotatedImage.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
    }

    // Fill the original image with example data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            image.pixels[i][j].r = 255;
            image.pixels[i][j].g = 255;
            image.pixels[i][j].b = 0;
        }
    }

    printf("Image:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("(%3d, %3d, %3d) ", image.pixels[i][j].r, image.pixels[i][j].g, image.pixels[i][j].b);
        }
        printf("\n");
    }

    // Rotate the image
    EDIT_Rotate(&image, &rotatedImage, angle);

    printf("Rotated Image:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("(%3d, %3d, %3d) ", rotatedImage.pixels[i][j].r, rotatedImage.pixels[i][j].g, rotatedImage.pixels[i][j].b);
        }
        printf("\n");
    }
}
*/