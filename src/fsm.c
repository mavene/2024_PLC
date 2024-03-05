/* Macro FSM - main flow of program here 
   (Might combine with main.. but we'll see...) */

typedef enum {
    START,
    IDLE,
    UPLOAD_MODE,
    PARSER_PPM,
    EDIT_MODE,
    EDIT_TRANSFORMATION,
    EDIT_ROTATION,
    EDIT_SCALING,
    EDIT_EDGE_DETECT,
    PREVIEW_MODE,
    DOWNLOAD_MODE,
    ERROR,
    END  
} State;

/*
    Define the FSM object as a struct.
    State: Current state of the FSM.
*/
typedef struct {
    State currentState;
} FSM;

/*
    Initialize the FSM before running an input string.
    Simply set the state attribute to ???.
*/
void initFSM(FSM *fsm) {
    fsm->currentState = START;
}