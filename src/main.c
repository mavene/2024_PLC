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
    printf("Goodbye~\n");
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
    //char* output;
    //Image* image;

    /* Test UI */
    //open_gtk_window("example.png");

    /* Test Conversion */
    //pngtoppm("example.png"); // example.png

    /* Test Parser */
    //image = ppmToMatrix("example.ppm");

    // /* Matrix to PPM */
    //matrixToPPM()
    // printMatrix(image);
    
    /* Test Conversion
    status = ppmtopng("example.ppm");
    if (status) {
        printf("Error with using the externals to convert to PNG.\n");
    }
    
    // Test Writer
    //output = matrixToPPM(transformImage); // image
    
    // Test Conversion
    //ppmtopng(output);
    //freeImage(transformImage);
    //freeImage(image);*/

    atexit(functionA);

    return 0; //status;
}