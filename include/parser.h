#ifndef PARSER_H
#define PARSER_H

#include "state.h"

/*
    Define the Pixel object as a struct.
    r: Red channel value.
    g: Green channel value.
    b: Blue channel value.
*/
typedef struct {
    int r;
    int g;
    int b;
} Pixel;

/*
    Define the Image object as a struct.
    width: Current image width.
    height: Current image height.
    max_val: Bit depth ceiling of image.
    pixels: 2D array storing pointers to Pixels (by row then, height)
*/
typedef struct {
    int width;
    int height;
    int max_val;
    Pixel **pixels;
} Image;

void initInput(char*);
void get_command(State, char*);
void get_values(State, int*, int, int);
int get_file(char*);
int pngtoppm(char*);
int ppmtopng(char*);
Image *createImage(int, int, int);
void freeImage(Image*);
void freeInnerImage(Image*);
Image* ppmToMatrix(char*);
char* matrixToPPM(Image*);
void printMatrix(Image*);

#endif