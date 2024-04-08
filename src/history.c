#include <stdio.h>
#include "history.h"
#include <stdlib.h>

void initHistory(History *h) {
    h->initStatus = 0;
    h->filePath = NULL;
    Image** currPtr = (Image**)malloc(sizeof(Image*));
    Image** transPtr = (Image**)malloc(sizeof(Image*));
    *currPtr = NULL;
    *transPtr = NULL;
    h->currentImage = currPtr;
    h->transformedImage = transPtr;
}

void undo(History* h) {
    /* Wipe transformedImage */
    printf("Undoing transformation\n");
    freeImage(*(h->transformedImage));
    *(h->transformedImage) = NULL;
}

void commit(History* h) {
    /* Wipe old image and replace with transformed image*/
    printf("Applying transformation\n");
    freeImage(*(h->currentImage));
    *(h->currentImage) = *(h->transformedImage);
    *(h->transformedImage) = NULL;
}


void wipe(History* h) {
    free(h->filePath);
    freeImage(*(h->currentImage));
    h->filePath = NULL;
    *(h->currentImage) = NULL;
    *(h->transformedImage) = NULL;
}

void freeHistory(History *h) {
    free(h->filePath);
    freeImage((*h->currentImage));
    freeImage((*h->transformedImage));
    free(h->currentImage);
    free(h->transformedImage);
    free(h);
}