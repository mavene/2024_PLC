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
    *currPtr = NULL;
    *transPtr = NULL;

    h->initStatus = 0;
    h->filePath = NULL;
    h->currentImage = currPtr;
    h->transformedImage = transPtr;
}

/* Wipe transformedImage and keep current working copy */
void undo(History* h) {
    printf("Undoing transformation\n");
    freeImage(*(h->transformedImage));
    *(h->transformedImage) = NULL;
}

/* Wipe current working copy and replace with transformed image*/
void commit(History* h) {
    printf("Applying transformation\n");
    freeImage(*(h->currentImage));
    *(h->currentImage) = *(h->transformedImage);
    *(h->transformedImage) = NULL;
}

/* Wipe old history data - reset*/
void wipe(History* h) {
    free(h->filePath);
    freeImage(*(h->currentImage));
    h->initStatus = 0;
    h->filePath = NULL;
    *(h->currentImage) = NULL;
    *(h->transformedImage) = NULL;
}

/* Frees history 
NOTE: Current Image, Transformed Image and file path is alrdy freed when we exit FSM */
void freeHistory(History *h) {
    free(h); 
}