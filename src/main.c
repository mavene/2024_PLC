/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"
#include "history.h"

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

    while(f->currentState != END) {
        transition(f, h, input);
    }
    END_FSM();
    atexit(functionA);

    free(input);
    freeHistory(h);
    freeFSM(f);

    atexit(functionA);

    return 0;
}