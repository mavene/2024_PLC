#ifndef HISTORY_H_ 
#define HISTORY_H_

#include "parser.h"

typedef struct {
    int initStatus;
    char* filePath;
    Image** currentImage;
    Image** transformedImage;
} History;

void initHistory(History*);
void undo(History*);
void redo(History*); 
void commit(History*);
void freeHistory(History*);

#endif