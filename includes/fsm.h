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
    EXIT  
} State;

typedef enum {
   UPLOAD_FAIL,
   PARSE_FAIL,
   DOWNLOAD_FAIL 
   /* PREVIEW_FAIL*/
} Error;

/*
    Define the FSM object as a struct.
    State: Current state of the FSM.
*/
typedef struct {
    State currentState;
    State prevState;
    Error errorCode;
} FSM;
