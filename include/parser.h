#ifndef PARSER_H
#define PARSER_H

#include "fsm.h"

typedef struct {
    int red;
    int green;
    int blue;
} Pixel;

typedef struct {
    int width;
    int height;
    Pixel **pixels;
} Image;

void get_command(State state, char* input);
Image *createImage(int width, int height);
void freeImage(Image *image);
void ppmToMatrix(const char *filename);
void printImage(Image *image);

#endif