#ifndef PARSER_H
#define PARSER_H

#include "state.h"

typedef struct {
    int r;
    int g;
    int b;
} Pixel;

typedef struct {
    int width;
    int height;
    int max_val;
    char *filename;
    Pixel **pixels;
} Image;

void initInput(char*);
void get_command(State, char*);
void get_values(State, int*, int, int);
int get_file(char*);
void strip_ext(char*);
int pngtoppm(char*);
int ppmtopng(char*);
Image *createImage(int, int, int, char*);
void freeImage(Image*);
void freeInnerImage(Image*);
Image* ppmToMatrix(char*);
char* matrixToPPM(Image*);
void printMatrix(Image*);

#endif