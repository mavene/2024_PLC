#include <math.h>
#include "parser.h"
#define PI 3.14159265
#define FILL 0
#define RADTOANG 57.29577951
Pixel bilinear_interpolate(double, double, long, long, Pixel**);
void EDIT_Rotate(const Image*, Image*, float);
void EDIT_Scale(const Image*, Image*, int);