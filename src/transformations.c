/* Basic editing features - rotate, scale, crop and edge detection */
#include <stdio.h>
#include <stdlib.h>
#include "transformations.h"
#include "parser.h"
/* NITHYA, SHIVANI */

/* Calculate new dimensions for rotation*/
void EDIT_CalcRotatedDimensions(const Image *image, int *rotatedWidth, int *rotatedHeight, float angle) {
    double radian_angle = angle / RADTOANG;
    double cosa = cos(radian_angle);
    double sina = sin(radian_angle);
    double m = image->width / 2.0;
    double n = image->height / 2.0;

    /* Calculate the coordinates of the four corners of the original image */
    double x0 = -m;
    double y0 = -n;
    double x1 = image->width - m;
    double y1 = -n;
    double x2 = -m;
    double y2 = image->height - n;
    double x3 = image->width - m;
    double y3 = image->height - n;

    /* Rotate the corner points */
    double rx0 = x0 * cosa - y0 * sina;
    double ry0 = x0 * sina + y0 * cosa;
    double rx1 = x1 * cosa - y1 * sina;
    double ry1 = x1 * sina + y1 * cosa;
    double rx2 = x2 * cosa - y2 * sina;
    double ry2 = x2 * sina + y2 * cosa;
    double rx3 = x3 * cosa - y3 * sina;
    double ry3 = x3 * sina + y3 * cosa;

    /* Calculate the bounding box dimensions */
    double minx = fmin(fmin(rx0, rx1), fmin(rx2, rx3));
    double maxx = fmax(fmax(rx0, rx1), fmax(rx2, rx3));
    double miny = fmin(fmin(ry0, ry1), fmin(ry2, ry3));
    double maxy = fmax(fmax(ry0, ry1), fmax(ry2, ry3));

    *rotatedWidth = (int)ceil(maxx - minx);
    *rotatedHeight = (int)ceil(maxy - miny);
}

/* Performs clockwise rotation on image */
void EDIT_Rotate(const Image *image, Image* rotatedImage, float angle) {
    double radian_angle = angle / RADTOANG;
    double cosa = cos(radian_angle);
    double sina = sin(radian_angle);
    double m = image->width / 2.0;
    double n = image->height / 2.0;
    int new_i, new_j;
    double m1 = rotatedImage->width / 2.0;
    double n1 = rotatedImage->height / 2.0;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            
            /* Calculate new pixel coordinates to swap */
            double tmpx = (double)(j) * cosa - (double)(i) * sina - (double)(m) * cosa + (double)(m) + (double)(n) * sina - m + m1 ;
            double tmpy = (double)(i) * cosa + (double)(j) * sina - (double)(m) * sina - (double)(n) * cosa + (double)(n) - n + n1;
            
            /* Round to nearest integer to get exact coordinates */
            new_j = (int)round(tmpx);
            new_i = (int)round(tmpy);

            /* Check if the new coordinates are within the image bounds */
            if (new_j < 0 || new_j >= rotatedImage->width || new_i < 0 || new_i >= rotatedImage->height) {
                rotatedImage->pixels[i][j].r = FILL;
                rotatedImage->pixels[i][j].g = FILL;
                rotatedImage->pixels[i][j].b = FILL;
            } else {
                rotatedImage->pixels[new_i][new_j] = image->pixels[i][j];
            }
        }
    }
}

/* Performs crop on image */
void EDIT_Crop(const Image *image, Image *transformedImage, int x, int y, int cropWidth, int cropHeight) {
    /* Retrieve dimensions of transformed image */
    int transformedWidth = cropWidth;
    int transformedHeight = cropHeight;

    /* Iterate over each pixel in the transformed image */
    for (int i = 0; i < transformedHeight; i++) {
        for (int j = 0; j < transformedWidth; j++) {
            /* Calculate original coordinates after considering cropping and moving */
            int originalX = j ;
            int originalY = i ;

            /* Check if the original coordinates are within the bounds of the cropped area */
            if (originalX >= 0 && originalX < cropWidth && originalY >= 0 && originalY < cropHeight) {
                /* Map cropped coordinates to the original image coordinates */
                int mappedX = x + originalX;
                int mappedY = y + originalY;

                /* Check if the mapped coordinates are within the bounds of the original image */
                if (mappedX >= 0 && mappedX < image->width && mappedY >= 0 && mappedY < image->height) {
                    /* Copy pixel from the original image */
                    transformedImage->pixels[i][j] = image->pixels[mappedY][mappedX];
                } else {
                    /* If the mapped coordinates are outside the image bounds, fill with FILL color */
                    transformedImage->pixels[i][j].r = FILL;
                    transformedImage->pixels[i][j].g = FILL;
                    transformedImage->pixels[i][j].b = FILL;
                }
            } else {
                /* If the original coordinates are outside the cropped area, fill with FILL color */
                transformedImage->pixels[i][j].r = FILL;
                transformedImage->pixels[i][j].g = FILL;
                transformedImage->pixels[i][j].b = FILL;
            }
        }
    }

    /* Update the width and height of the transformed image */
    transformedImage->width = transformedWidth;
    transformedImage->height = transformedHeight;
}


/* Performs scaling on image */
void EDIT_Scale(const Image *image, Image *scaledImage, int scale, int corner) {

    /* Retrieve dimensions of the scaled image */
    int scaledRows = image->height ;
    int scaledCols = image->width ;

    /* Variables to store starting coordinates */
    int startX = 0, startY = 0;

    /* Determine starting coordinates based on corner */
    switch (corner) {
        case 0:  /* Top-left corner */
            startX = 0;
            startY = 0;
            break;
        case 1:  /* Top-right corner */
            startX = image->width - scaledCols / scale;
            startY = 0;
            break;
        case 2:  /* Bottom-left corner */
            startX = 0;
            startY = image->height - scaledRows / scale;
            break;
        case 3:  /* Bottom-right corner */
            startX = image->width - scaledCols / scale;
            startY = image->height - scaledRows / scale;
            break;
        default: /* Default to top-left corner */
            startX = 0;
            startY = 0;
            break;
    }

    /* Iterate over each pixel in the scaled image */
    for (int i = 0; i <scaledRows; i++) {
        for (int j = 0; j < scaledCols; j++) {
            /* Calculate corresponding pixel coordinates in the original image */
            int pre_i = startY + i / scale;
            int pre_j = startX + j / scale;

            /* Ensure pre_i and pre_j are within the bounds of the original image */
            if (pre_i >= 0 && pre_i < image->height && pre_j >= 0 && pre_j < image->width) {
                 scaledImage->pixels[i][j] = image->pixels[pre_i][pre_j];
            }
        }
    }
}

/* Calculate convolution of image */
Pixel applySobel(const Image *image, int x, int y) {
    /* Sobel operator kernels for horizontal and vertical direction */
    int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    /* Iterate through each pixel */
    int sumX = 0, sumY = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int pixelX = x + j;
            int pixelY = y + i;

            /* Apply kernel to colour channels - assume linear */
            if (pixelX >= 0 && pixelX < image->width && pixelY >= 0 && pixelY < image->height) {
                sumX += image->pixels[pixelY][pixelX].r * kernelX[i][j];
                sumX += image->pixels[pixelY][pixelX].g * kernelX[i][j];
                sumX += image->pixels[pixelY][pixelX].b * kernelX[i][j];

                sumY += image->pixels[pixelY][pixelX].r * kernelY[i][j];
                sumY += image->pixels[pixelY][pixelX].g * kernelY[i][j];
                sumY += image->pixels[pixelY][pixelX].b * kernelY[i][j];
            }
        }
    }

    /* Calculate gradient magnitude */
    int magnitude = (int)sqrt(sumX * sumX + sumY * sumY);

    /* Set threshold to determine edges from background */
    if (magnitude > 128) {
        return (Pixel){255, 255, 255}; /* White for edges */
    } else {
        return (Pixel){FILL, FILL, FILL}; /* Black for background */
    }
}

/* Performs edge detection on image */
void EDIT_Edgedetection(const Image *image, Image *edgeImage) {
    /* Retrieve dimensions of image */
    edgeImage->width = image->width;
    edgeImage->height = image->height;

    /* Call Sobel helper above */
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            edgeImage->pixels[i][j] = applySobel(image, j, i);
        }
    }
}

/* Perform grayscale mapping filter */
void EDIT_GrayscaleMap(const Image *image, const Image *edgeImage) {
     for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            float average = (0.3*image->pixels[i][j].r + 0.59*image->pixels[i][j].g + 0.1*image->pixels[i][j].b) / 3;
            edgeImage->pixels[i][j].r = average;
            edgeImage->pixels[i][j].g = average;
            edgeImage->pixels[i][j].b = average;
        }
    }
}

/* Perform grayscale mapping filter */
void EDIT_RedMap(const Image *image, const Image *transformImage) {
     for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            transformImage->pixels[i][j].r = image->pixels[i][j].r;
            transformImage->pixels[i][j].g = 0;
            transformImage->pixels[i][j].b = 0;
        }
    }
}


/* Perform grayscale mapping filter */
void EDIT_GreenMap(const Image *image, const Image *transformImage) {
     for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            transformImage->pixels[i][j].r = 0;
            transformImage->pixels[i][j].g = image->pixels[i][j].g;
            transformImage->pixels[i][j].b = 0;
        }
    }
}


/* Perform grayscale mapping filter */
void EDIT_BlueMap(const Image *image, const Image *transformImage) {
     for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            transformImage->pixels[i][j].r = 0;
            transformImage->pixels[i][j].g = 0;
            transformImage->pixels[i][j].b = image->pixels[i][j].b;
        }
    }
}
