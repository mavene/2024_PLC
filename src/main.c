/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"

/* Perform clean up here */
void functionA() {
    printf("Caught the exit! Goodbye~\n");
}

int main(int argc, char **argv) {
    FSM* f = (FSM*) malloc(sizeof(FSM));
    char* input = (char*) malloc(sizeof(char) * 1);
    int status;

    status = gtk_app(argc, argv);
    printf("UI upp.\n");

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

    return status;
}