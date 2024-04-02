/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"
#include "parser.h"
#include "transformations.h"

/* Perform clean up here */
void functionA() {
    printf("Caught the exit! Goodbye~\n");
}

int main(int argc, char **argv) {
    //FSM* f = (FSM*) malloc(sizeof(FSM));
    //char* input = (char*) malloc(sizeof(char) * 1);
    //int status;
    char* output;
    Image* image;

    /* Test UI */
    //open_gtk_window("example.png");

    /* Test Conversion */
    pngtoppm("image.png"); // example.png

    /* Test Parser */
    image = ppmToMatrix("example.ppm");

    // /* Matrix to PPM */
    // matrixToPPM()
    // printMatrix(image);
    
    /* Test Conversion
    status = ppmtopng("example.ppm");
    if (status) {
        printf("Error with using the externals to convert to PNG.\n");
    }*/

    /* Test Rotation -> must be separate from translate and scale for now */ 
    Image* transformImage = createImage(image->width, image->height, image->max_val, image->filename);
    //EDIT_Rotate_new(image, rotateImage, 360); // does not work
    //EDIT_Rotate_new(image, rotateImage, 270); // does not work
    //EDIT_Rotate_new(image, rotateImage, 180); // kinda works?
    //EDIT_Rotate_new(image, rotateImage, 90); // kinda works?
    //EDIT_Rotate(image,rotateImage, 90);
    //EDIT_Scale(image, transformImage, 2);
    EDIT_Transformation(image, transformImage, 50, 50, 50, 50, 0, 1); //some issus with this but its all good
    //EDIT_Edgedetection(image, transformImage);

    /* Test Writer */
    output = matrixToPPM(transformImage); // image
    
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
    freeImage(transformImage);
    freeImage(image);

    return 0; //status;
}