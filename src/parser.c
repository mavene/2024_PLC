/* Parser of all user inputs */
#include <stdio.h>
#include "fsm.h"
/* MICHELLE */

/* Retrieve command */
void get_command(State state, char* input) {
    
    *input = getchar();
    getchar(); /* Consumes the pesky \n. Need to find a better way */
    if (*input != EOF) {
        if (state == IDLE && (*input != '1' && *input != '2')) {
            printf("Please enter only 1 (Continue) or 2 (Exit)\n");
            get_command(state, input); /* Loop back */
        }
    }
}

/* Handle any numerical input for edit commands here - angles/degrees whatever */

/* Our bitmap parser here huehue*/
/* output shd be a 2D array with each pointer pointing to each row's pixels*/