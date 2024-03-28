#include <stdio.h>
#include "fsm.h"
#include "parser.h"
/* DIAN */

/* Initialize FSM */
void initFSM(FSM *fsm) {
   fsm->currentState = START;
   fsm->prevState = START;
}

void transition(FSM* fsm, char* input) {
   int statusCode = 0;

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
         break;
      case '2':
         /* Goes to END */
         fsm->currentState = END;
         break;
      }
      fsm->prevState = IDLE;
   }
   else if (fsm->currentState == UPLOAD_MODE) {
      statusCode = UPLOAD_FSM();
      
      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = UPLOAD_FAIL; /* Might need to check statusCode to differentiate between Parser and Upload failures */
         /* Error State Handler*/
      } else {
         fsm->currentState = PREVIEW_MODE;
      }
      fsm->prevState = UPLOAD_MODE;
   }
   else if (fsm->currentState == PREVIEW_MODE) {
      statusCode = PREVIEW_FSM(fsm, input);

      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = PREVIEW_FAIL;
      } else {
         switch (*input)
         {
         case 'Y':
            if (fsm->prevState == UPLOAD_MODE || 
                  fsm->prevState == EDIT_TRANSLATION ||
                  fsm->prevState == EDIT_SCALING ||
                  fsm->prevState == EDIT_ROTATION ||
                  fsm->prevState == EDIT_EDGE_DETECT) {
               fsm->currentState = EDIT_MODE;
            } else if (fsm->prevState == EDIT_MODE) {
               fsm->currentState = DOWNLOAD_MODE;
            }
            break;
         case 'N':
            if (fsm->prevState == UPLOAD_MODE) {
               fsm->currentState = UPLOAD_MODE;
            } else if (fsm->prevState == EDIT_MODE || 
                        fsm->prevState == EDIT_TRANSLATION || 
                        fsm->prevState == EDIT_ROTATION || 
                        fsm->prevState == EDIT_SCALING || 
                        fsm->prevState == EDIT_EDGE_DETECT) {
               fsm->currentState = EDIT_MODE;
               /* Undo handler here */
            }
            break;
         }
      }
      fsm->prevState = PREVIEW_MODE;
   }
   else if (fsm->currentState == EDIT_MODE) {
      EDIT_FSM(fsm, input);

      switch (*input) 
      {
      case '1':
         fsm->currentState = EDIT_TRANSLATION;
         break;
      case '2':
         fsm->currentState = EDIT_SCALING;
         break;
      case '3':
         fsm->currentState = EDIT_ROTATION;
         break;
      case '4':
         fsm->currentState = EDIT_EDGE_DETECT;
         break;
      case '5':
         fsm->currentState = PREVIEW_MODE;
         break;
      }
      fsm->prevState = EDIT_MODE;
   }
   else if (fsm->currentState == EDIT_TRANSLATION) {
      EDIT_translation_FSM(); /* maybe need statusCode and defo inputs */

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_TRANSLATION;
   }
   else if (fsm->currentState == EDIT_SCALING) {
      EDIT_scaling_FSM(); /* maybe need statusCode and defo inputs */

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_SCALING;
   }
   else if (fsm->currentState == EDIT_ROTATION) {
      EDIT_rotation_FSM(); /* maybe need statusCode and defo inputs */

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_ROTATION;
   }
   else if (fsm->currentState == EDIT_EDGE_DETECT) {
      EDIT_edgeDetect_FSM(); /* maybe need statusCode and defo inputs */

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_EDGE_DETECT;
   }
   else if (fsm->currentState == DOWNLOAD_MODE) {
      statusCode = DOWNLOAD_FSM(); /* maybe need input on where to download image and UI prompt */

      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = DOWNLOAD_FAIL;
      } else {
         fsm->currentState = START;
      }
      fsm->prevState = DOWNLOAD_MODE;
   }
   else if (fsm->currentState == ERROR) {
      switch (fsm->errorCode)
      {
      case UPLOAD_FAIL:
         printf("Something went wrong with the upload...");
         fsm->currentState = fsm->prevState;
         break;
      case PREVIEW_FAIL:
         printf("Something went wrong with the preview...");
         fsm->currentState = fsm->prevState;
         break;
      case DOWNLOAD_FAIL:
         printf("Something weent wrong with the download...");
         fsm->currentState = fsm->prevState;
         break;
      default:
         printf("Something went wrong...");
         break;
      }
   }
}

/* START state handler*/
void START_FSM() {
   printf("Press 1 to continue and upload an image or 2 to exit.\n");
}

/* IDLE state handler*/
void IDLE_FSM(FSM* f, char* input) {
   get_command(f->currentState, input);
}

/* UPLOAD state handler */
int UPLOAD_FSM() {
   printf("Add all upload functions here\n");
   /* Call all upload_mode functions
      1. Upload image (user provides file path in terminal)
      2. Parsing, must return a status code*/
   
   return 0;
}

/* PREVIEW state handler */
int PREVIEW_FSM(FSM* f, char* input) {
   printf("Add all preview functions here\n");
   printf("Would you like to continue with this image? Y (Yes) or N (No).\n");
   /* Call all preview_mode functions
      1. Calls ui functions with the file path and/or converts pixel arrays to proper ppm->png format
      2. Asks for input (below)*/

   /* Insert file_path function here*/
   get_command(f->currentState, input);
   
   return 0;
}

/* EDIT state handler */
void EDIT_FSM(FSM* f, char* input) {
   printf("Editing tools:\n1. Translation (Crop)\n2. Scale (Zoom)\n3. Rotation\n4. Edge Detection\n5. Preview Mode\n");
   get_command(f->currentState, input);
}

/* EDIT translation state handler */
void EDIT_translation_FSM() {
   printf("Add all translation functions here.\n");
}

/* EDIT scaling state handler */
void EDIT_scaling_FSM() {
   printf("Add all scaling functions here.\n");
}

/* EDIT rotation state handler */
void EDIT_rotation_FSM() {
   printf("Add all rotation functions here.\n");
}

/* EDIT edge detection state handler */
void EDIT_edgeDetect_FSM() {
   printf("Add all edge detection functions here.\n");
}

int DOWNLOAD_FSM() {
   printf("Add all download functions here.\n");
   
   return 0;
}

/* END state handler */
void END_FSM() {
   printf("Exiting program.\n"); /*Replace with atexit with clean-up*/
}