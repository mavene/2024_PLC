#include <math.h>
#include "parser.h"
#define PI 3.14159265
#define FILL 0
#define RADTOANG 57.29577951
Pixel applySobel(const Image*, int, int y);
Pixel bilinear_interpolate(double, double, long, long, Pixel**);
void EDIT_Rotate_1(const Image*, Image*, float);
void EDIT_Rotate(const Image*, Image*, float);
void EDIT_Rotate_new(const Image*, Image*, float);
void EDIT_Transformation(const Image*, Image*, int, int, int cropWidth, int cropHeight, int dx, int dy);
void EDIT_Scale(const Image *image, Image *scaledImage, int scale, int corner);
void EDIT_Edgedetection(const Image *image, Image *edgeImage);
void EDIT_CalcRotatedDimensions(const Image *image, int*, int*, float );
