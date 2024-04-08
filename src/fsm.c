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
      statusCode = UPLOAD_FSM(fsm, history, input); /* Retrieves file path from user */
      
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
      statusCode = PREVIEW_FSM(fsm, history, input); /* Display transformed/current image to user through UI */

      if (statusCode) {
         /* Error State Handler*/
         fsm->currentState = ERROR;
         fsm->errorCode = PREVIEW_FAIL;
      } else {
         switch (*input)
         {
         case 'Y':
            if (fsm->prevState == EDIT_MODE ||
                  fsm->prevState == EDIT_TRANSLATION ||
                  fsm->prevState == EDIT_SCALING ||
                  fsm->prevState == EDIT_ROTATION ||
                  fsm->prevState == EDIT_EDGE_DETECT) {
                  fsm->currentState = EDIT_MODE;
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
      int statusCode = PARSE_PPM_FSM(fsm, history, input); /* Parse PNG to PPM to to matrix/image struct */

      if (statusCode) {
         fsm->currentState = ERROR;
         fsm->errorCode = PARSE_FAIL;
      } else {
         fsm->currentState = EDIT_MODE;
      }
      fsm->prevState = PARSER_PPM;
   }
   else if (fsm->currentState == EDIT_MODE) {
      EDIT_FSM(fsm, input); /* Display edit tools to user */

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
      case '6':
         fsm->currentState = DOWNLOAD_MODE;
      }
      fsm->prevState = EDIT_MODE;
   }
   else if (fsm->currentState == EDIT_TRANSLATION) {
      EDIT_translation_FSM(fsm, history);  /* Apply crop transformation */
      history->initStatus = 1;

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_TRANSLATION;
   }
   else if (fsm->currentState == EDIT_SCALING) {
      EDIT_scaling_FSM(fsm, history); /* Apply scale transformation */
      history->initStatus = 1;

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_SCALING;
   }
   else if (fsm->currentState == EDIT_ROTATION) {
      EDIT_rotation_FSM(fsm, history); /* Apply rotation transformation */
      history->initStatus = 1;

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_ROTATION;
   }
   else if (fsm->currentState == EDIT_EDGE_DETECT) {
      EDIT_edgeDetect_FSM(history); /* Apply edge detection transformation */
      history->initStatus = 1;

      fsm->currentState = PREVIEW_MODE;
      fsm->prevState = EDIT_EDGE_DETECT;
   }
   else if (fsm->currentState == DOWNLOAD_MODE) {
      statusCode = DOWNLOAD_FSM(history); /* Clean up artifacts and download PNG */

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
         printf("Something went wrong with the upload...\n");
         fsm->currentState = END;
         break;
      case PARSE_FAIL:
         printf("Something went wrong while parsing...\n");
         fsm->currentState = END;
         break;
      case PREVIEW_FAIL:
         printf("Something went wrong with the preview...\n");
         fsm->currentState = END;
         break;
      case DOWNLOAD_FAIL:
         printf("Something went wrong with the download...\n");
         fsm->currentState = END;
         break;
      default:
         printf("Something went wrong...\n");
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
      2. Saves path to history */
   int statusCode = 0;
   char* input_file = (char*)malloc(sizeof(char)*256);
   
   printf("File path of image to process (PNGs only):\n");
   statusCode = get_file(input_file);
   h->filePath = input_file;

   return statusCode;
}

/* PREVIEW state handler */
int PREVIEW_FSM(FSM* f, History* h, char* input) {
   /* Call all preview_mode functions
      1. Calls UI functions with the correct file path
      2. Prompts user for which version to keep */
   int statusCode = 0;
   char* output_ppm;

   if (h->initStatus && *(h->transformedImage)) {
      printf("Found transformed image\n");
      output_ppm = matrixToPPM(*(h->transformedImage));
      statusCode = ppmtopng(output_ppm);
      open_gtk_window("./media/output.png");

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
               statusCode = ppmtopng(output_ppm);
               break;
            }
      } 
   } else if (h->initStatus) {
      open_gtk_window("./media/output.png");
      *input = 'Y';
   } else {
      open_gtk_window(h->filePath);
      *input = 'Y';
   }
   
   return statusCode;
}

int PARSE_PPM_FSM(FSM* f, History* h, char* input) {
   /* Call all parsing functions
      1. Calls UI functions with the file path
      2. Confirms user that the image selected is correct s*/
   int statusCode = 0;
   Image* image;

   statusCode = pngtoppm(h->filePath);
   image = ppmToMatrix("./media/working.ppm");
   *(h->currentImage) = image;
   return statusCode;
}

/* EDIT state handler */
void EDIT_FSM(FSM* f, char* input) {
   /* Call all edit functions
      1. User chooses edit tool */
   printf("Editing tools:\n1. Translate (Crop)\n2. Scale (Zoom)\n3. Rotate\n4. Edge Detection\n5. Preview\n6. Download\n");
   get_command(f->currentState, input);
}

/* EDIT translation state handler */
void EDIT_translation_FSM(FSM* f, History* h) {
   /* Call all translation functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */
   Image* transformImage;
   int *x_start = (int*)malloc(sizeof(int));
   int *y_start = (int*)malloc(sizeof(int));
   int *x_length = (int*)malloc(sizeof(int));
   int *y_length = (int*)malloc(sizeof(int));

   printf("Current dimensions (x y): %d x %d\n", (*h->currentImage)->width -1, (*h->currentImage)->height -1);

   printf("Starting from which X coordinate?\n");
   get_values(f->currentState, x_start, 0, (*h->currentImage)->width -1);
   printf("Starting from which Y coordinate?\n");
   get_values(f->currentState, y_start, 0, (*h->currentImage)->height -1);
   printf("Crop width?\n");
   get_values(f->currentState, x_length, 0, (*h->currentImage)->width -1 - *x_start);
   printf("Crop height?\n");
   get_values(f->currentState, y_length, 0, (*h->currentImage)->height -1 - *y_start);

   transformImage = createImage(*x_length, *y_length, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   
   EDIT_Crop(*(h->currentImage), transformImage, *x_start, *y_start, *x_length, *y_length);
   *(h->transformedImage) = transformImage;

   free(x_start);
   free(y_start);
   free(x_length);
   free(y_length);
}

/* EDIT scaling state handler */
void EDIT_scaling_FSM(FSM* f, History* h) {
   /* Call all scaling functions
      1. Create image to track transformation
      2. Update pointer in History to transformed image */
   Image* transformImage;
   int *scale = (int*)malloc(sizeof(int));
   int *corner = (int*)malloc(sizeof(int));

   transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);

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
   Image* transformImage;
   int *new_width = (int*)malloc(sizeof(int));
   int *new_height = (int*)malloc(sizeof(int));
   int *angle = (int*)malloc(sizeof(int));
   *new_width = (*h->currentImage)->width;
   *new_height = (*h->currentImage)->height;

   printf("Rotate by how much? (90, 180, 270 or its negative equivalents)\n");
   get_values(f->currentState, angle, 0, 0);

   EDIT_CalcRotatedDimensions(*(h->currentImage), new_width, new_height, *angle);
   transformImage = createImage(*new_width, *new_height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Rotate(*(h->currentImage), transformImage, *angle);
   *(h->transformedImage) = transformImage;
   
   free(new_width);
   free(new_height);
   free(angle);
}

/* EDIT edge detection state handler */
void EDIT_edgeDetect_FSM(History* h) {
   Image* transformImage;
   transformImage = createImage((*h->currentImage)->width, (*h->currentImage)->height, (*h->currentImage)->max_val, (*h->currentImage)->filename);
   EDIT_Edgedetection(*(h->currentImage), transformImage);
   *(h->transformedImage) = transformImage;
}

int DOWNLOAD_FSM(History* h) {
   /* Download latest version, reset history and prompt user to check output file */
   int statusCode = 0;
   char* output_ppm;

   output_ppm = matrixToPPM(*(h->currentImage));
   statusCode = ppmtopng(output_ppm);

   wipe(h);
   printf("Check the /media folder for the output.png\n");
   
   return statusCode;
}

/* END state handler */
void END_FSM() {
   printf("Exiting program.\n"); /*Replace with atexit with clean-up*/
}