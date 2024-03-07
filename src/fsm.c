#include <stdio.h>
#include "fsm.h"
#include "parser.h"

/* Initialize FSM */
void initFSM(FSM *fsm) {
   fsm->currentState = START;
   fsm->prevState = START;
}

void transition(FSM* fsm, char* input) {
   if (fsm->currentState == START) {
      START_FSM(); /* Prints out welcome message*/

      fsm->currentState = IDLE; /* Goes to IDLE, no action */
      fsm->prevState = START;
   }
   else if (fsm->currentState == IDLE) {
      IDLE_FSM(fsm, input); /* Retrieves command */

      switch (*input) 
      {
      case '1':
         /* Goes to UPLOAD */
         fsm->currentState = UPLOAD_MODE;
         fsm->prevState = IDLE;
         break;
      case '2':
         /* Goes to END */
         fsm->currentState = END;
         fsm->prevState = IDLE;
      }
   }
}

/* START state handler*/
void START_FSM() {
   printf("Welcome.\nPress 1 to continue and upload an image or 2 to exit.\n");
   /* transition(f, input); */
}

/* IDLE state handler*/
void IDLE_FSM(FSM* f, char* input) {
   get_command(f->currentState, input);
   /* transition(f, input); */
}

/* END state handler */
void END_FSM() {
   printf("Exiting program. Goodbye~\n"); /*Replace with atexit with clean-up*/
}