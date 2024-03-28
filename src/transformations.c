/* Basic editing features - rotate, scale and translate, redo and undo */
/* NITHYA*/
#include <stdio.h>
#include <stdlib.h>
#include "transformations.h"
#include <parser.h>

void EDIT_Translate(Pixel ***preImage,Pixel ***translatedImage){
    
}

void rotateImage(Pixel **image, Pixel **rotatedImage, int rows, int cols, float angle) {
    double radian_angle = angle/ RADTOANG;
    double cosa = cos(radian_angle);
    double sina = sin(radian_angle);
    double m = cols / 2.0;
    double n = rows / 2.0;
    int i,j,new_i, new_j;
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            /*Apply rotation*/
            double tmpx = (double)(j) * cosa - (double)(i) * sina - (double)(m) * cosa + (double)(m) + (double)(n) * sina;
            double tmpy = (double)(i) * cosa + (double)(j) * sina - (double)(m) * sina - (double)(n) * cosa + (double)(n);

            /* Round to nearest integer to get pixel coordinates */
            new_j = (int)round(tmpx);
            new_i = (int)round(tmpy);
            
            /* Check if the new coordinates are within the image bounds */
            if(new_j < 0 ||new_j >= cols ||new_i < 0 ||new_i >= rows) {
                rotatedImage[i][j].red = FILL;
                rotatedImage[i][j].green = FILL;
                rotatedImage[i][j].blue = FILL;
            } else {
                rotatedImage[i][j].red = image[new_i][new_j].red;
                rotatedImage[i][j].green = image[new_i][new_j].green;
                rotatedImage[i][j].blue = image[new_i][new_j].blue;
            }
        }
    }
}

void EDIT_Scale(Pixel ***preImage,Pixel ***scaledImage){

}

/*
int main() {
    Example image dimensions
    int rows = 14;
    int cols = 7;
    int i, j;
    Angle of rotation (in degrees) 
    float angle = 45.0;
    Allocate memory for the original and rotated images 
    Pixel **image = (Pixel **)malloc(rows * sizeof(Pixel *));
    Pixel **rotatedImage = (Pixel **)malloc(rows * sizeof(Pixel *));
    for (i = 0; i < rows; i++) {
        image[i] = (Pixel *)malloc(cols * sizeof(Pixel));
        rotatedImage[i] = (Pixel *)malloc(cols * sizeof(Pixel));
    }
    Fill the original image with example data 
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            image[i][j].r = 255; 
            image[i][j].g = 255;
            image[i][j].b = 0;
        }
    }
    Display the original image 
    printf("Image:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("(%3d, %3d, %3d) ", image[i][j].r, image[i][j].g, image[i][j].b);
        }
        printf("\n");
    }
    Rotate the image 
    rotateImage(image, rotatedImage, rows, cols, angle);
    Display the rotated image 
    printf("Rotated Image:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("(%3d, %3d, %3d) ", rotatedImage[i][j].r, rotatedImage[i][j].g, rotatedImage[i][j].b);
        }
        printf("\n");
    }
    Free allocated memory 
    for (i = 0; i < rows; i++) {
        free(image[i]);
        free(rotatedImage[i]);
    }
    free(image);
    free(rotatedImage);

    return 0;
}
*/