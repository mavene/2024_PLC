#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "history.h" // parser.h
#include "transformations.h"
#include "ui.h"
/* DIAN */

/* Initialize FSM and its History */
void initFSM(FSM *fsm) {
   fsm->currentState = START;
   fsm->prevState = START;
}

void transition(FSM* fsm, History* history, char* input) {
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
      statusCode = UPLOAD_FSM(fsm, history, input);
      
      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = UPLOAD_FAIL;
         /* Error State Handler*/
      } else {
         fsm->currentState = PREVIEW_MODE;
      }
      fsm->prevState = UPLOAD_MODE;
   }
   else if (fsm->currentState == PREVIEW_MODE) {
      statusCode = PREVIEW_FSM(fsm, history, input);

      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = PREVIEW_FAIL;
      } else {
         switch (*input)
         {
         case 'Y':
            if (fsm->prevState == EDIT_TRANSLATION || /*UPLOAD_MODE*/
                  fsm->prevState == EDIT_SCALING ||
                  fsm->prevState == EDIT_ROTATION ||
                  fsm->prevState == EDIT_EDGE_DETECT) {
                  fsm->currentState = EDIT_MODE;
            } else if (fsm->prevState == EDIT_MODE) {
               fsm->currentState = DOWNLOAD_MODE;
            } else if (fsm->prevState == UPLOAD_MODE) {
               fsm->currentState = PARSER_PPM;
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
   else if (fsm->currentState == PARSER_PPM) {
      int statusCode = PARSE_PPM_FSM(fsm, history, input);

      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = PARSE_FAIL;
      } else {
         fsm->currentState = EDIT_MODE;
      }
      fsm->prevState = PARSER_PPM;
   }
   else if (fsm->currentState == EDIT_MODE) {
      EDIT_FSM(fsm, input);
      history->initStatus = 1;

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
      EDIT_translation_FSM(history); 

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_TRANSLATION;
   }
   else if (fsm->currentState == EDIT_SCALING) {
      EDIT_scaling_FSM(history); 

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_SCALING;
   }
   else if (fsm->currentState == EDIT_ROTATION) {
      EDIT_rotation_FSM(history); 

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_ROTATION;
   }
   else if (fsm->currentState == EDIT_EDGE_DETECT) {
      EDIT_edgeDetect_FSM(history);

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_EDGE_DETECT;
   }
   else if (fsm->currentState == DOWNLOAD_MODE) {
      statusCode = DOWNLOAD_FSM();

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

void freeFSM(FSM *f) {
    free(f);
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
int UPLOAD_FSM(FSM* f, History* h, char* input) {
   /* Call all upload_mode functions
      1. User provides filename input
      2. Saves path */
   printf("Add all upload functions here\n");
   char* file = "example.png";
   printf("Got a file! %s\n", file);
   h->filePath = file;
   printf("History obtained file at %s\n", h->filePath);

   return 0;
}

/* PREVIEW state handler */
int PREVIEW_FSM(FSM* f, History* h, char* input) {
   /* Call all preview_mode functions
      1. Calls UI functions with the correct file path
      2. Confirms user that the image selected is correct s*/
   printf("Add all preview functions here\n");
   char* output_ppm;
   if (h->initStatus && *(h->transformedImage)) {
      printf("Found transformed image\n");
      output_ppm = matrixToPPM(*(h->transformedImage));
      ppmtopng(output_ppm);
      open_gtk_window("output.png");
   } else if (h->initStatus) {
      open_gtk_window("output.png");
   } else {
      open_gtk_window(h->filePath); // TODO: Add in statusCode handler
   }
   
   printf("Would you like to continue with this image? Y (Yes) or N (No).\n");
   get_command(f->currentState, input);
   if (*(h->transformedImage)) {
      switch (*input) { 
         case 'Y':
            commit(h);
            break;
         case 'N':
            undo(h); /*TODO forgot to update the png*/
            break;
      }
   }
   
   return 0;
}

int PARSE_PPM_FSM(FSM* f, History* h, char* input) {
   /* Call all parsing functions
      1. Calls UI functions with the file path
      2. Confirms user that the image selected is correct s*/
   //printf("Add all parsing functions here\n");
   Image* image;
   pngtoppm("example.png");
   image = ppmToMatrix("example.ppm");
   *(h->currentImage) = image;
   // printMatrix(h->currentImage); works!
   return 0;
}

/* EDIT state handler */
void EDIT_FSM(FSM* f, char* input) {
   /* Call all edit functions
      1. User chooses edit tool */
   printf("Editing tools:\n1. Translation (Crop)\n2. Scale (Zoom)\n3. Rotation\n4. Edge Detection\n5. Preview Mode\n");
   get_command(f->currentState, input);
}

/* EDIT translation state handler */
void EDIT_translation_FSM(History* h) {
   /* Call all translation functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */
   //printf("Add all translation functions here.\n");
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Transformation(*(h->currentImage), transformImage, 10, 10, 50, 50, 0, 1);
   *(h->transformedImage) = transformImage;
}

/* EDIT scaling state handler */
void EDIT_scaling_FSM(History* h) {
   //printf("Add all scaling functions here.\n");
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Scale(*(h->currentImage), transformImage, 2);
   *(h->transformedImage) = transformImage;
}

/* EDIT rotation state handler */
void EDIT_rotation_FSM(History* h) {
   //printf("Add all rotation functions here.\n");
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Rotate(*(h->currentImage), transformImage, 180);
   *(h->transformedImage) = transformImage;
}

/* EDIT edge detection state handler */
void EDIT_edgeDetect_FSM(History* h) {
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Edgedetection(*(h->currentImage), transformImage);
   *(h->transformedImage) = transformImage;
}

int DOWNLOAD_FSM() {
   /* TODO: Reset the entire application */
   printf("Add all download functions here.\n");
   printf("Check the output.png!\n");
   
   return 0;
}

/* END state handler */
void END_FSM() {
   printf("Exiting program.\n"); /*Replace with atexit with clean-up*/
}