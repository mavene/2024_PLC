#include <math.h>
#include "parser.h"
#define PI 3.14159265
#define FILL 0
#define RADTOANG 57.29577951
Pixel applySobel(const Image *image, int x, int y);
Pixel bilinear_interpolate(double x, double y, long cols, long rows, Pixel **the_image);
void EDIT_Rotate(const Image *image, Image *rotatedImage, float angle);
void EDIT_Transformation(const Image *image, Image *transformedImage, int x, int y, int cropWidth, int cropHeight, int dx, int dy);
void EDIT_Scale(const Image *image, Image *scaledImage, int scale); 
void EDIT_Edgedetection(const Image *image, Image *edgeImage);
