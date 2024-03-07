#ifndef FSM_H_ 
#define FSM_H_

typedef enum {
    START,
    IDLE,
    UPLOAD_MODE,
    PARSER_PPM,
    EDIT_MODE,
    EDIT_TRANSLATION,
    EDIT_ROTATION,
    EDIT_SCALING,
    EDIT_EDGE_DETECT,
    PREVIEW_MODE,
    DOWNLOAD_MODE,
    ERROR,
    END  
} State;

typedef enum {
   UPLOAD_FAIL,
   PARSE_FAIL,
   PREVIEW_FAIL,
   DOWNLOAD_FAIL
} Error;

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
void transition(FSM*, char*);
void START_FSM(void);
void IDLE_FSM(FSM*, char*);
int UPLOAD_FSM(void);
int PREVIEW_FSM(FSM*, char*);
void EDIT_FSM(FSM*, char*);
void EDIT_translation_FSM(void);
void EDIT_scaling_FSM(void);
void EDIT_rotation_FSM(void);
void EDIT_edgeDetect_FSM(void);
int DOWNLOAD_FSM(void);
void END_FSM(void);

#endif