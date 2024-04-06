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
    // printf("Transformed now at %p\n", (void*) *(h->transformedImage));
    // printf("Current now at %p\n", (void*) *(h->currentImage));
    freeImage(*(h->transformedImage));
    *(h->transformedImage) = NULL;
    // printf("Transformed now at %p\n", (void*) *(h->transformedImage));
    // printf("Current now at %p\n", (void*) *(h->currentImage));
}

void redo(History* h) {
    printf("Hello");
    /*Swap the transformed to current*/
}

void commit(History* h) {
    /* Wipe old image and replace with transformed image*/
    printf("Applying transformation\n");
    // printf("Transformed now at %p\n", (void*) *(h->transformedImage));
    // printf("Current now at %p\n", (void*) *(h->currentImage));
    freeImage(*(h->currentImage));
    *(h->currentImage) = *(h->transformedImage);
    *(h->transformedImage) = NULL;
    // printf("Transformed now at %p\n", (void*) *(h->transformedImage));
    // printf("Current now at %p\n", (void*) *(h->currentImage));
}

void freeHistory(History *h) {
    free(h->filePath);
    freeImage((*h->currentImage));
    freeImage((*h->transformedImage));
    free(h->currentImage);
    free(h->transformedImage);
    free(h);
}