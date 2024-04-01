/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"
#include "parser.h"

/* Perform clean up here */
void functionA() {
    printf("Caught the exit! Goodbye~\n");
}

int main(int argc, char **argv) {
    FSM* f = (FSM*) malloc(sizeof(FSM));
    char* input = (char*) malloc(sizeof(char) * 1);
    //int status;
    char* output;
    Image* image;

    /* Test UI */
    open_gtk_window("example.png");

    /* Test Conversion */
    pngtoppm("example.png");

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

    /* Test Writer */
    output = matrixToPPM(image);
    
    /* Test Conversion */
    ppmtopng(output);

    /* Test FSM */
    initFSM(f); 
    memset(input, ' ', sizeof(char));

    printf("Welcome.\n");
    while(f->currentState != END) {
        transition(f, input);
    }
    END_FSM();

    atexit(functionA);
    free(f);
    free(input);

    return 0; //status;
}