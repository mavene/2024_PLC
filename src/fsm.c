/* Macro FSM - main flow of program here 
   (Might combine with main.. but we'll see...) */

#include "fsm.h"
/* DIAN by this week*/

/*
    Initialize the FSM before running an input string.
    Simply set the state attribute to ???.
*/
void initFSM(FSM *fsm) {
    fsm->currentState = IDLE;
    fsm->prevState = START;
}

void processInput(FSM *fsm, char input) {
   // if condition to check user input
   if (fsm->currentState == IDLE) {
      switch (input)
      {
      case 'Y':
         /* Goes to UPLOAD */
         fsm->currentState = UPLOAD_MODE;
         fsm->prevState = IDLE;
         break;
      case 'N':
         /* Goes to EXIT*/
         fsm->currentState = EXIT;
         break;
      default:
         /* Please input only 'Y' or 'N' */
         printf("Please only input Y or N.");
         break;
      }
   }
   else if (fsm->currentState == UPLOAD_MODE) {
      /* Call all upload_mode functions
      1. Upload image (user provides file path in terminal)
      2. Parser stuff, must return a status code
      3. If status code is 1 above, go into ERROR otherwise swap to PREVIEW_MODE, set prevState to UPLOAD
      */
      int statusCode = 0;

      /* sytatusCode = function to be called here that checks if succeeds or not^*/

      if (statusCode) {
         /* If status code returned > 0, its encountered an error...*/
         fsm->currentState = ERROR;
         fsm->prevState = UPLOAD_MODE;
      } else {
         fsm->currentState = PREVIEW_MODE;
         fsm->prevState = UPLOAD_MODE;
      }
     
   }
   else if (fsm->currentState == EDIT_MODE){
    /*Show a menu with inputs and what each letter means*/
    printf("Please input a T,R,S,E,D or P \nT: Translation \nR: Rotation \nS: Scaling \nE:Edge Detect \nD:Download \nP:Preview \n");
    switch (input){
      case 'T':
         fsm->currentState=EDIT_TRANSLATION;
         fsm->prevState = EDIT_MODE;
         /* Call all translation functions here*/
         break;
      case 'R':
         fsm->currentState=EDIT_ROTATION;
         fsm->prevState = EDIT_MODE;
         /* Call all rotation functions here*/
         break;
      case 'S':
         fsm->currentState=EDIT_SCALING;
         fsm->prevState = EDIT_MODE;
         /* Call all scale functions here*/
         break;
      case 'E':
         fsm->currentState=EDIT_EDGE_DETECT;
         fsm->prevState = EDIT_MODE;
         /* Call all edge detection functions here*/
         break;
      case 'D':
         fsm->currentState=DOWNLOAD_MODE;
         fsm->prevState = EDIT_MODE;
         /* Call all edge detection functions here*/
         break;
      case 'P':
         fsm->currentState=PREVIEW_MODE;
         fsm->prevState = EDIT_MODE;
         /* Call all edge detection functions here*/
         break;
      default:
         printf("Please input a T , R , S , E , D or P");
    }
    
   }
   else if (fsm->currentState==EDIT_TRANSLATION){
      int statusCode = 0;
      /* sytatusCode = function to be called here that checks if succeeds or not^*/
      if (statusCode) {
         /* If status code returned > 0, its encountered an error...*/
         fsm->currentState = ERROR;
         fsm->prevState = EDIT_TRANSLATION;
      } else {
         fsm->currentState = EDIT_TRANSLATION;
         fsm->prevState = EDIT_MODE;
      }

   }
   else if (fsm->currentState==EDIT_ROTATION){
      int statusCode = 0;
      /* sytatusCode = function to be called here that checks if succeeds or not^*/
      if (statusCode) {
         /* If status code returned > 0, its encountered an error...*/
         fsm->currentState = ERROR;
         fsm->prevState = EDIT_ROTATION;
      } else {
         fsm->currentState = EDIT_ROTATION;
         fsm->prevState = EDIT_MODE;
      }
      
   }
   else if (fsm->currentState==EDIT_SCALING){
      int statusCode = 0;
      /* sytatusCode = function to be called here that checks if succeeds or not^*/
      if (statusCode) {
         /* If status code returned > 0, its encountered an error...*/
         fsm->currentState = ERROR;
         fsm->prevState = EDIT_SCALING;
      } else {
         fsm->currentState = EDIT_SCALING;
         fsm->prevState = EDIT_MODE;
      }
      
   }
   else if (fsm->currentState==EDIT_EDGE_DETECT){
      int statusCode = 0;
      /* sytatusCode = function to be called here that checks if succeeds or not^*/
      if (statusCode) {
         /* If status code returned > 0, its encountered an error...*/
         fsm->currentState = ERROR;
         fsm->prevState = EDIT_EDGE_DETECT;
      } else {
         fsm->currentState = EDIT_EDGE_DETECT;
         fsm->prevState = EDIT_MODE;
      }
      
   }
   else if (fsm->currentState == PREVIEW_MODE) {
      switch (input) {
         /* Make sure redo and undo only appears here*/
         case 'Y':
            /* Check prevState, if its Upload mode, go to PARSE_PPM */
            if (fsm->prevState == UPLOAD_MODE) {
               fsm->currentState = PARSER_PPM;
               fsm->prevState == PREVIEW_MODE;
            /* Check prevState, if its any Edit mode, go to EDIT_MODE*/
            } else if (fsm->prevState > 4 || fsm->prevState < 6) {
               fsm->prevState = 
               fsm->currentState = EDIT_MODE;
            }
            break;
         case 'N':
            /* Check prevState, if its Upload mode, go to UPLOAD_MODE*/
            if (fsm->prevState == UPLOAD_MODE) {
               fsm->currentState = UPLOAD_MODE;
            }
            /* Check prevState, if its any Edit mode, undo all changes and go to EDIT_MODE*/
            break;

      }
   }
   else if (fsm->currentState == PARSER_PPM) {
      int 
   }
   else if (fsm->currentState == DOWNLOAD_MODE) {
      /* generate PNG file */
      printf("Continue editing this image?\n");
      switch(input) {
         case 'Y': 
            fsm->currentState == EDIT_MODE;
            break;
         case 'N':
            fsm->currentState == IDLE;
            break;
         default:
            break;
      }
   }

   // call the respective functions
}
