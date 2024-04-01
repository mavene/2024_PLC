#ifndef PARSER_H
#define PARSER_H

#include "fsm.h"

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

void get_command(State, char*);
int pngtoppm(char*);
int ppmtopng(char*);
Image *createImage(int, int, int, char*);
void freeImage(Image*);
Image* ppmToMatrix(char*);
char* matrixToPPM(Image*);
void printMatrix(Image*);

#endif