/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "ui.h"
#include "history.h"

/* Print goodbye cuz why not */
void goodbye() {
    printf("Goodbye~\n");
}


int main(int argc, char **argv) {
    /* Initialize all submodules */
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
    printf("Welcome.\n");
    initFSM(f); 
    initHistory(h);
    initInput(input);

    /* Run program */
    while(f->currentState != END) {
        transition(f, h, input);
    }
    END_FSM();

    /* Final clean-up */
    free(input);
    freeHistory(h);
    freeFSM(f);

    /* Goodbye message */
    atexit(goodbye);

    return 0;
}