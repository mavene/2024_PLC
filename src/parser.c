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
            printf("Please enter only these options below\n1. Upload file\n2. Exit program\n");
            get_command(state, input); /* Loop back */
        } 
        else if (state == PREVIEW_MODE && (*input != 'Y' && *input != 'N')) {
            printf("Please enter only Y (Yes) or N (No)\n");
            get_command(state, input); /* Loop back */
        }
        else if (state == EDIT_MODE && (*input != '1' && 
                                        *input != '2' && 
                                        *input != '3' &&
                                        *input != '4' &&
                                        *input != '5')) {
            printf("Please enter only these options below\n1. Translation (Crop)\n2. Scale (Zoom)\n3. Rotation\n4. Edge Detection\n5.Preview Mode\n");
            get_command(state, input); /* Loop back */
        }
    }
}

/* Handle any numerical input for edit commands here - angles/degrees whatever */

/* Our bitmap parser here huehue*/
/* output shd be a 2D array with each pointer pointing to each row's pixels*/