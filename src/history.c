#include <stdio.h>
#include "history.h"
#include <stdlib.h>
/* DIAN */

/* Initialize History */
void initHistory(History *h) {
    Image** currPtr;
    Image** transPtr;
    currPtr = (Image**)malloc(sizeof(Image*));
    transPtr = (Image**)malloc(sizeof(Image*));
    *currPtr = NULL; /* Set overall pointer to NULL (no image) */
    *transPtr = NULL; /* Set overall pointer to NULL (no image) */

    h->initStatus = 0;
    h->filePath = NULL;
    h->currentImage = currPtr;
    h->transformedImage = transPtr;
}

/* Wipe transformedImage and keep current working copy */
void undo(History* h) {
    printf("Undoing transformation\n");
    freeImage(*(h->transformedImage)); /* Frees image from overall pointer */
    *(h->transformedImage) = NULL; /* Set overall pointer to NULL (no image) */
}

/* Wipe current working copy and replace with transformed image*/
void commit(History* h) {
    printf("Applying transformation\n");
    freeImage(*(h->currentImage)); /* Frees image from overall pointer */
    *(h->currentImage) = *(h->transformedImage); /* Swap pointers */
    *(h->transformedImage) = NULL; /* Set overall pointer to NULL (no image) */
}

/* Wipe old history data - reset*/
void wipe(History* h) {
    free(h->filePath);
    freeImage(*(h->currentImage)); /* Frees image from overall pointer */
    h->initStatus = 0;
    h->filePath = NULL;
    *(h->currentImage) = NULL; /* Set overall pointer to NULL (no image) */
    *(h->transformedImage) = NULL; /* Set overall pointer to NULL (no image) */
}

/* Frees history 
NOTE: Current Image, Transformed Image and File Path is alrdy freed when we exit FSM */
void freeHistory(History *h) {
    free(h); 
}