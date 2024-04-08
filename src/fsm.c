#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "history.h"
#include "transformations.h"
#include "ui.h"
/* DIAN */

/* Initialize FSM */
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
         /* Error State Handler*/
         fsm->currentState = ERROR;
         fsm->errorCode = UPLOAD_FAIL;
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
            if (fsm->prevState == EDIT_TRANSLATION ||
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
      EDIT_translation_FSM(fsm, history); 

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_TRANSLATION;
   }
   else if (fsm->currentState == EDIT_SCALING) {
      EDIT_scaling_FSM(fsm, history); 

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_SCALING;
   }
   else if (fsm->currentState == EDIT_ROTATION) {
      EDIT_rotation_FSM(fsm, history); 

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
         printf("Something went wrong with the download...");
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
   //printf("Add all upload functions here\n");
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
   //printf("Add all preview functions here\n");
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
            undo(h);
            printf("Reverting image\n");
            output_ppm = matrixToPPM(*(h->currentImage));
            ppmtopng(output_ppm);
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
   //printMatrix(image);
   *(h->currentImage) = image;
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
void EDIT_translation_FSM(FSM* f, History* h) {
   /* Call all translation functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */

    
   int *x_start = (int*)malloc(sizeof(int));
   int *y_start = (int*)malloc(sizeof(int));
   int *x_length = (int*)malloc(sizeof(int));
   int *y_length = (int*)malloc(sizeof(int));

   printf("Current dimensions (x y): %d x %d", (*h->currentImage)->width -1, (*h->currentImage)->height -1);

   printf("Starting from which X coordinate?\n");
   get_values(f->currentState, x_start, 0, (*h->currentImage)->width -1);
   printf("Starting from which Y coordinate?\n");
   get_values(f->currentState, y_start, 0, (*h->currentImage)->height -1);
   printf("Crop width?\n");
   get_values(f->currentState, x_length, 0, (*h->currentImage)->width -1 - *x_start);
   printf("Crop height?\n");
   get_values(f->currentState, y_length, 0, (*h->currentImage)->height -1 - *y_start);

   Image* transformImage = createImage(*x_length, *y_length, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   
   EDIT_Crop(*(h->currentImage), transformImage, *x_start, *y_start, *x_length, *y_length);
   *(h->transformedImage) = transformImage;

   free(x_start);
   free(y_start);
   free(x_length);
   free(y_length);
}

/* EDIT scaling state handler */
void EDIT_scaling_FSM(FSM* f, History* h) {
   /* Call all translation functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);

   int *scale = (int*)malloc(sizeof(int));
   int *corner = (int*)malloc(sizeof(int));

   printf("Scale by how much? (2-5x)\n");
   get_values(f->currentState, scale, 2, 5);
   printf("Starting which corner?\n1. Top-left\n2. Top-right\n3. Bottom-left\n4. Bottom-right\n");
   get_values(f->currentState, corner, 0, 0);

   EDIT_Scale(*(h->currentImage), transformImage, *scale, *corner);
   *(h->transformedImage) = transformImage;

   free(scale);
   free(corner);
}

/* EDIT rotation state handler */
void EDIT_rotation_FSM(FSM* f, History* h) {
   /* Call all translation functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */

   int *new_width = (int*)malloc(sizeof(int));
   int *new_height = (int*)malloc(sizeof(int));
   int *angle = (int*)malloc(sizeof(int));
   *new_width = (*h->currentImage)->width;
   *new_height = (*h->currentImage)->height;

   printf("Rotate by how much? (90, 180, 270 or its negative equivalents)\n");
   get_values(f->currentState, angle, 0, 0);

   EDIT_CalcRotatedDimensions(*(h->currentImage), new_width, new_height, *angle);
   Image* transformImage = createImage(*new_width, *new_height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Rotate(*(h->currentImage), transformImage, *angle);
   *(h->transformedImage) = transformImage;
   
   free(new_width);
   free(new_height);
   free(angle);
}

/* EDIT edge detection state handler */
void EDIT_edgeDetect_FSM(History* h) {
   Image* transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Edgedetection(*(h->currentImage), transformImage);
   *(h->transformedImage) = transformImage;
}

int DOWNLOAD_FSM() {
   /* TODO: Reset the entire application */
   printf("Check the output.png!\n");
   
   return 0;
}

/* END state handler */
void END_FSM() {
   printf("Exiting program.\n"); /*Replace with atexit with clean-up*/
}