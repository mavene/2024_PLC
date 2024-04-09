#ifndef FSM_H_ 
#define FSM_H_

#include "history.h"

/*
    Define the FSM object as a struct.
    currentState: Current state of the FSM.
    prevState: Previous state of the FSM.
    errorCode: Error code stored in FSM.
*/
typedef struct {
    State currentState;
    State prevState;
    Error errorCode;
} FSM;

void initFSM(FSM*);
void transition(FSM*, History*, char*);
void freeFSM(FSM*);
void START_FSM(void);
void IDLE_FSM(FSM*, char*);
int UPLOAD_FSM(FSM*, History*, char*);
int PREVIEW_FSM(FSM*, History*, char*);
int PARSE_PPM_FSM(FSM*, History*, char*);
void EDIT_FSM(FSM*, char*);
void EDIT_translation_FSM(FSM*, History*);
void EDIT_scaling_FSM(FSM*, History*);
void EDIT_rotation_FSM(FSM*, History*);
void EDIT_edgeDetect_FSM(History*);
void EDIT_greyMap_FSM(History*);
int DOWNLOAD_FSM(History*);
void END_FSM(void);

#endif