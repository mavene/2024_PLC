/* Basic editing features - rotate, scale and translate, redo and undo */
/* NITHYA*/
#include <stdio.h>
#include <stdlib.h>
#include "transformations.h"
#include "parser.h"

Pixel applySobel(const Image *image, int x, int y) {
    // Sobel operator kernels for horizontal and vertical gradients
    int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int sumX = 0, sumY = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int pixelX = x + j;
            int pixelY = y + i;

            // Apply kernel to pixel
            if (pixelX >= 0 && pixelX < image->width && pixelY >= 0 && pixelY < image->height) {
                sumX += image->pixels[pixelY][pixelX].r * kernelX[i + 1][j + 1];
                sumY += image->pixels[pixelY][pixelX].r * kernelY[i + 1][j + 1];
            }
        }
    }

    // Calculate gradient magnitude
    int magnitude = (int)sqrt(sumX * sumX + sumY * sumY);

    // Apply thresholding to convert to binary edge image
    if (magnitude > 128) {
        return (Pixel){255, 255, 255}; // White for edges
    } else {
        return (Pixel){FILL, FILL, FILL}; // Black for background
    }
}

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

void EDIT_Rotate_1(const Image *image, Image *rotatedImage, float angle) {
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

void EDIT_CalcRotatedDimensions(const Image *image, int *rotatedWidth, int *rotatedHeight, float angle) {
    double radian_angle = angle / RADTOANG;
    double cosa = cos(radian_angle);
    double sina = sin(radian_angle);
    double m = image->width / 2.0;
    double n = image->height / 2.0;

    // Calculate the coordinates of the four corners of the original image
    double x0 = -m;
    double y0 = -n;
    double x1 = image->width - m;
    double y1 = -n;
    double x2 = -m;
    double y2 = image->height - n;
    double x3 = image->width - m;
    double y3 = image->height - n;

    // Rotate the corner points
    double rx0 = x0 * cosa - y0 * sina;
    double ry0 = x0 * sina + y0 * cosa;
    double rx1 = x1 * cosa - y1 * sina;
    double ry1 = x1 * sina + y1 * cosa;
    double rx2 = x2 * cosa - y2 * sina;
    double ry2 = x2 * sina + y2 * cosa;
    double rx3 = x3 * cosa - y3 * sina;
    double ry3 = x3 * sina + y3 * cosa;

    // Calculate the bounding box dimensions
    double minx = fmin(fmin(rx0, rx1), fmin(rx2, rx3));
    double maxx = fmax(fmax(rx0, rx1), fmax(rx2, rx3));
    double miny = fmin(fmin(ry0, ry1), fmin(ry2, ry3));
    double maxy = fmax(fmax(ry0, ry1), fmax(ry2, ry3));

    *rotatedWidth = (int)ceil(maxx - minx);
    *rotatedHeight = (int)ceil(maxy - miny);
}

void EDIT_Rotate(const Image *image, Image* RotatedImage,  int *width, int *height, float angle) {
    // Calculate the new dimensions for the rotated image
    // int rotatedWidth, rotatedHeight;

    // Allocate memory for the rotated image if it's not already allocated
    // if (rotatedImage->pixels == NULL) {
    //     freeImage(rotatedImage);
        //*rotatedImage = *createImage(rotatedWidth, rotatedHeight, image->max_val, image->filename);
    //}

    double radian_angle = angle / RADTOANG;
    double cos_radian = cos(radian_angle);
    double sin_radian = sin(radian_angle);
    double x_center = floor(image->width / 2);
    double y_center = floor(image->height / 2);
    int x, y, new_x, new_y;

    for (y = 0; y < image->height; y++) {
        for (x = 0; x < image->width; x++) {
            // Apply rotation
            double x_tmp = (double)(x) * cos_radian - (double)(y) * sin_radian - (double)(x_center) * cos_radian + (double)(y_center) * sin_radian + x_center;
            double y_tmp = (double)(y) * cos_radian + (double)(x) * sin_radian - (double)(x_center) * sin_radian - (double)(y_center) * sin_radian + y_center;

            /* Round to nearest integer to get pixel coordinates */
            new_x = floor(x_tmp);
            new_y = floor(y_tmp);

            // Check if the new coordinates are within the image bounds
            if (new_x >= 0 && new_x < *width && new_y >= 0 && new_y < *height) {
                // Update the pixel in the rotated image with the pixel from the original image
                RotatedImage->pixels[y][x] = image->pixels[new_y][new_x];
            } else {
                // If the new coordinates are outside the image bounds, fill with FILL color
                RotatedImage->pixels[y][x].r = FILL;
                RotatedImage->pixels[y][x].g = FILL;
                RotatedImage->pixels[y][x].b = FILL;
            }
        }
    }
}

void EDIT_Rotate_new(const Image *image, Image *rotatedImage, float angle) {
    double radian_angle = angle / 180.0 * PI;
    double cos_radian = cos(radian_angle);
    double sin_radian = sin(radian_angle);
    double x_center = floor(image->width / 2);
    double y_center = floor(image->height / 2);
    int x, y, new_x, new_y;

    for (y = 0; y < image->height; y++) {
        for (x = 0; x < image->width; x++) {
            // Apply rotation
            // Slight Truncation
            //double x_tmp = (double)(x - x_center) * cos_radian - (double)(y - y_center) * sin_radian + x_center;
            //double y_tmp = (double)(y - y_center) * cos_radian + (double)(x - x_center) * sin_radian + y_center;

            double x_tmp = (double)(x) * cos_radian - (double)(y) * sin_radian - (double)(x_center) * cos_radian + (double)(y_center) * sin_radian + x_center; //+ x_center; works with 90 and 180
            double y_tmp = (double)(y) * cos_radian + (double)(x) * sin_radian - (double)(x_center) * sin_radian - (double)(y_center) * sin_radian + y_center; //+ y_center *2; works with 90 and 180

            /* Round to nearest integer to get pixel coordinates */
            new_x = floor(x_tmp);
            new_y = floor(y_tmp);

            //printf("Width: From %d -> %d\nHeight: From %d -> %d\n", x, new_x, y, new_y);

            // Check if the new coordinates are within the image bounds
            if (new_x < 0  || new_y < 0 || new_x >= image->width || new_y >= image->height) {
                // replace with interpolation
                rotatedImage->pixels[y][x].r = FILL;
                rotatedImage->pixels[y][x].g = FILL;
                rotatedImage->pixels[y][x].b = FILL;
            } else {
                rotatedImage->pixels[y][x] = image->pixels[new_y][new_x];
            }
        }
    }
}

void EDIT_Transformation(const Image *image, Image *transformedImage, int x, int y, int cropWidth, int cropHeight, int dx, int dy) {
    // Calculate dimensions of the transformed image
    int transformedWidth = cropWidth;
    int transformedHeight = cropHeight;

    // Iterate over each pixel in the transformed image
    for (int i = 0; i < transformedHeight; i++) {
        for (int j = 0; j < transformedWidth; j++) {
            // Calculate original coordinates after considering cropping and moving
            int originalX = j + dx;
            int originalY = i + dy;

            // Check if the original coordinates are within the bounds of the cropped area
            if (originalX >= 0 && originalX < cropWidth && originalY >= 0 && originalY < cropHeight) {
                // Map cropped coordinates to the original image coordinates
                int mappedX = x + originalX;
                int mappedY = y + originalY;

                // Check if the mapped coordinates are within the bounds of the original image
                if (mappedX >= 0 && mappedX < image->width && mappedY >= 0 && mappedY < image->height) {
                    // Copy pixel from the original image
                    transformedImage->pixels[i][j] = image->pixels[mappedY][mappedX];
                } else {
                    // If the mapped coordinates are outside the image bounds, fill with FILL color
                    transformedImage->pixels[i][j].r = FILL;
                    transformedImage->pixels[i][j].g = FILL;
                    transformedImage->pixels[i][j].b = FILL;
                }
            } else {
                // If the original coordinates are outside the cropped area, fill with FILL color
                transformedImage->pixels[i][j].r = FILL;
                transformedImage->pixels[i][j].g = FILL;
                transformedImage->pixels[i][j].b = FILL;
            }
        }
    }

    // Update the width and height of the transformed image
    transformedImage->width = transformedWidth;
    transformedImage->height = transformedHeight;
}



void EDIT_Scale(const Image *image, Image *scaledImage, int scale) {
    // Calculate dimensions of the scaled image
    int scaledRows = image->height;
    int scaledCols = image->width;

    // Iterate over each pixel in the scaled image
    for (int i = 0; i < scaledRows; i++) {
        for (int j = 0; j < scaledCols; j++) {
            // Map pixel coordinates back to original image with scaling
            int pre_i = i / scale;
            int pre_j = j / scale;

            scaledImage->pixels[i][j] = image->pixels[pre_i][pre_j];
        }
    }

    scaledImage->width = scaledCols;
    scaledImage->height = scaledRows;
}

void EDIT_Edgedetection(const Image *image, Image *edgeImage){
    edgeImage->width = image->width;
    edgeImage->height = image->height;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            edgeImage->pixels[i][j] = applySobel(image, j, i);
        }
    }
}

/*
int main() {
    Example image dimensions
    int rows = 14;
    int cols = 6;
    // Angle of rotation (in degrees)
    float angle = 90.0;
    // Scaling factor
    int scale = 2;
    // Parameters for cropping and moving
    int x = 0;
    int y = 0;
    int cropWidth = 3;
    int cropHeight = 3;
    int dx = 0;
    int dy = 1;

    // Allocate memory for the original and rotated images
    Image image, rotatedImage, scaledImage, transformedImage,edgeImage;
    image.width = cols;
    image.height = rows;
    image.pixels = (Pixel **)malloc(rows * sizeof(Pixel *));
    rotatedImage.width = cols;
    rotatedImage.height = rows;
    rotatedImage.pixels = (Pixel **)malloc(rows * sizeof(Pixel *));
    scaledImage.width = cols ;
    scaledImage.height = rows ;
    scaledImage.pixels = (Pixel **)malloc((rows) * sizeof(Pixel *));
    transformedImage.width = cropWidth;
    transformedImage.height = cropHeight;
    transformedImage.pixels = (Pixel **)malloc(cropHeight * sizeof(Pixel *));
    edgeImage.width = cols;
    edgeImage.height = rows;
    edgeImage.pixels = (Pixel **)malloc(rows * sizeof(Pixel *));
    
    for (int i = 0; i < rows; i++) {
        image.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
        rotatedImage.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
        scaledImage.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
        edgeImage.pixels[i] = (Pixel *)malloc(cols * sizeof(Pixel));
    }
    for (int i = 0; i < cropHeight; i++) {
        transformedImage.pixels[i] = (Pixel *)malloc(cropWidth * sizeof(Pixel));
    }

    // Fill the original image with example data (create a diagonal line)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == j || i==j+1 || i==j+2) {
                image.pixels[i][j].r = 155;
                image.pixels[i][j].g = 155;
                image.pixels[i][j].b = 155;
            } else {
                image.pixels[i][j].r = 0;
                image.pixels[i][j].g = 0;
                image.pixels[i][j].b = 0;
            }
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
    
    // Apply transformation (crop and move) to the rotated image
    EDIT_Transformation(&image, &transformedImage, x, y, cropWidth, cropHeight, dx, dy);

    printf("Transformed Image:\n");
    for (int i = 0; i < cropHeight; i++) {
        for (int j = 0; j < cropWidth; j++) {
            printf("(%3d, %3d, %3d) ", transformedImage.pixels[i][j].r, transformedImage.pixels[i][j].g, transformedImage.pixels[i][j].b);
        }
        printf("\n");
    }
    // Scale the image
    EDIT_Scale(&image, &scaledImage, scale);

    printf("Scaled Image:\n");
    for (int i = 0; i < scaledImage.height; i++) {
        for (int j = 0; j < scaledImage.width; j++) {
            printf("(%3d, %3d, %3d) ", scaledImage.pixels[i][j].r, scaledImage.pixels[i][j].g, scaledImage.pixels[i][j].b);
        }
        printf("\n");
    }
    // Apply edge detection
    EDIT_Edgedetection(&image, &edgeImage);

    printf("Edge Detected Image:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("(%3d, %3d, %3d) ", edgeImage.pixels[i][j].r, edgeImage.pixels[i][j].g, edgeImage.pixels[i][j].b);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(image.pixels[i]);
        free(rotatedImage.pixels[i]);
        free(scaledImage.pixels[i]);
        free(edgeImage.pixels[i]);
    }
    for (int i = 0; i < cropHeight; i++) {
        free(transformedImage.pixels[i]);
    }
    
    free(image.pixels);
    free(rotatedImage.pixels);
    free(transformedImage.pixels);
    free(scaledImage.pixels);
    free(edgeImage.pixels);
    }*/