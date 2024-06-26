/* Enumerate different FSM states. */
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
    EDIT_GREYMAP,
    EDIT_COLOURMAP,
    PREVIEW_MODE,
    DOWNLOAD_MODE,
    ERROR,
    END  
} State;

/* Enumerate different Error states. */
typedef enum {
   UPLOAD_FAIL,
   PARSE_FAIL,
   PREVIEW_FAIL,
   DOWNLOAD_FAIL
} Error;