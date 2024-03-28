#include "parser.h"
#define PI 3.14159265
#define FILL 0
#define RADTOANG 57.29577951
Pixel bilinear_interpolate(double x, double y, long cols, long rows, Pixel **the_image);
void EDIT_Rotate(Pixel **image, Pixel **rotatedImage, int rows, int cols, float angle);
void EDIT_Scale(Pixel **image, Pixel **scaledImage, int preRows, int preCols, int scale);