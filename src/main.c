/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"
#include "history.h"
// #include "transformations.h"

/* Perform clean up here */
void functionA() {
    printf("Caught the exit! Goodbye~\n");
}

int main(int argc, char **argv) {
    FSM* f = (FSM*) malloc(sizeof(FSM));
    if (f == NULL) {
        printf("Error with allocating FSM.\n");
    }
    History* h = (History*) malloc(sizeof(History));
    if (h == NULL) {
        printf("Error with allocating History.\n");
    }
    char* input = (char*) malloc(sizeof(char));
    if (input == NULL) {
        printf("Error with allocating Input.\n");
    }

    /* Initialise */
    printf("Welcome.\n");
    initFSM(f); 
    initHistory(h);
    initInput(input); 
    //memset(input, ' ', sizeof(char));

    while(f->currentState != END) {
        transition(f, h, input);
    }
    END_FSM();
    atexit(functionA);

    free(input);
    freeHistory(h);
    freeFSM(f);

    //int status;
    char* output;
    Image* image;
    Image* rotatedImage;
    // Image* transformImage;
    /* Test UI */
    //open_gtk_window("example.png");

    /* Test Conversion */
    pngtoppm("example.png"); // example.png

    /* Test Parser */
    image = ppmToMatrix("example.ppm");
    image->pixels[214][230].r = 0;
    image->pixels[214][230].g = 0;
    image->pixels[214][230].b = 255;

    image->pixels[214][233].r = 0;
    image->pixels[214][233].g = 0;
    image->pixels[214][233].b = 255;
   // /* Matrix to PPM */
    // matrixToPPM()
    //printMatrix(image);
    
    /* Test Conversion
    status = ppmtopng("example.ppm");
    if (status) {
        printf("Error with using the externals to convert to PNG.\n");
    }*/

    /* Test Rotation -> smust be separate from translate and scale for now */ 
    // calculate new width and height based off the prev
    int *new_width = (int*)malloc(sizeof(int));
    int *new_height = (int*)malloc(sizeof(int));
    *new_width = image->width;
    *new_height = image->height;

    // printf("%d %d \n", *new_width, *new_height);

    EDIT_CalcRotatedDimensions(image, new_width, new_height, 270); 
    // printf("%d %d \n", *new_width, *new_height);

    
    rotatedImage = createImage(*new_width, *new_height, image->max_val, image->filename); 

    // //rotatedImage = createImage(image->width, image->height, image->max_val, image->filename);
    // //rotatedImage = NULL;
    // //EDIT_Rotate_new(image, rotateImage, 360); // does not work
    // //EDIT_Rotate_new(image, rotateImage, 270); // does not work
    // //EDIT_Rotate_new(image, rotateImage, 180); // kinda works?
    // //EDIT_Rotate_new(image, rotateImage, 90); // kinda works?
    EDIT_Rotate(image, rotatedImage, 270); 

    //EDIT_Rotate(image,rotateImage, 90);
    //EDIT_Scale(image, transformImage, 2);
    // EDIT_Transformation(image, transformImage, 50, 50, 50, 50, 0, 1); //some issus with this but its all good
    //EDIT_Edgedetection(image, transformImage);

    /* Test Writer */
    output = matrixToPPM(rotatedImage); // image
    
    /* Test Conversion */
    ppmtopng(output);

    /* Test FSM
    initFSM(f); 
    memset(input, ' ', sizeof(char));

    printf("Welcome.\n");
    while(f->currentState != END) {
        transition(f, input);
    }
    END_FSM();

    atexit(functionA);
    free(f);
    free(input);*/
    freeImage(rotatedImage);
    freeImage(image);

    return 0; //status;
}