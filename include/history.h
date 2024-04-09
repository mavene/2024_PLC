#ifndef HISTORY_H_ 
#define HISTORY_H_

#include "parser.h"

/*
    Define the History object as a struct.
    initStatus: Switch to determine if history is tracking.
    filePath: Original image file path.
    currentImage: Overall pointer to current Image* pointer
    transformedImage: Overall pointer to transformed Image* pointer
*/
typedef struct {
    int initStatus;
    char* filePath;
    Image** currentImage;
    Image** transformedImage;
} History;

void initHistory(History*);
void undo(History*);
void wipe(History*);
void commit(History*);
void freeHistory(History*);

#endif