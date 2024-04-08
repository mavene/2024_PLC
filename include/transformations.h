#include <math.h>
#include "parser.h"
#define PI 3.14159265
#define FILL 0
#define RADTOANG 57.29577951
void EDIT_CalcRotatedDimensions(const Image*, int*, int*, float );
void EDIT_Rotate(const Image*, Image*, float);
void EDIT_Crop(const Image*, Image*, int, int, int, int);
void EDIT_Scale(const Image*, Image*, int, int);
// void adjustLuminosity(const Image*, const Image*);
Pixel applySobel(const Image*, int, int y);
void EDIT_Edgedetection(const Image*, Image*);