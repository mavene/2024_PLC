/* Entrypoint of program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "parser.h"

/* Perform clean up here */
void functionA() {
    printf("Caught the exit!\n");
}

int main() {
    FSM* f = (FSM*) malloc(sizeof(FSM));
    char* input = (char*) malloc(sizeof(char) * 1);

    initFSM(f); 
    memset(input, ' ', sizeof(char));

    while(f->currentState != END) {
        transition(f, input);
    }
    END_FSM();

    atexit(functionA);
    free(f);
    free(input);

    return 0;
}