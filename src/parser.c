/* Parser of all user inputs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "fsm.h"
/* MICHELLE */

/* Initialize input for commands */
void initInput(char* i) {
    memset(i, ' ', sizeof(char));
}

/* Retrieve command */
void get_command(State state, char* input) {
    *input = getchar();
    getchar(); /* Consumes the pesky \n. Need to find a better way */
    if (*input != EOF) {
        if (state == IDLE && (*input != '1' && *input != '2')) {
            printf("Please enter only these options below\n1. Upload file\n2. Exit program\n");
            while((*input = getchar()) != EOF && *input != '\n');
            get_command(state, input); /* Loop back */
        } 
        else if (state == PREVIEW_MODE && (*input != 'Y' && *input != 'N')) {
            printf("Please enter only Y (Yes) or N (No)\n");
            while((*input = getchar()) != EOF && *input != '\n');
            get_command(state, input); /* Loop back */
        }
        else if (state == EDIT_MODE && (*input != '1' && 
                                        *input != '2' && 
                                        *input != '3' &&
                                        *input != '4' &&
                                        *input != '5' &&
                                        *input != '6' &&
                                        *input != '7' &&
                                        *input != '8')) {
            printf("Please enter only these options below\n1. Translation (Crop)\n2. Scale (Zoom)\n3. Rotation\n4. Edge Detection\n5.Preview Mode\n");
            while((*input = getchar()) != EOF && *input != '\n');
            get_command(state, input); /* Loop back */
        }
    }
}

/* Retrieve numerical inputs for edit commands */
void get_values(State state, int *input, int floor, int ceil) {
    char line[6];
    fgets(line, 6, stdin);
    sscanf(line, "%d", input);

    if (state == EDIT_ROTATION) {
        if (*input != 90 && *input != 180 && *input != 270 && *input != -90 && *input != -180 && *input != -270) {
            printf("Please only enter the following values: 90 180 270 -90 -180 -270\n");
            get_values(state, input, floor, ceil); /* Loop back */
        } 
    } else if (state == EDIT_SCALING) {
        if (ceil != 0 || floor != 0) {
            if (*input < floor || *input > ceil) {
                printf("Please only enter a value between %d and %d\n", floor, ceil);
                get_values(state, input, floor, ceil); /* Loop back */
            }
        } else {
            if (*input != 1 && *input != 2 && *input != 3 && *input != 4) {
                printf("Please enter only these options below\n1. Top-left\n2. Top-right\n3. Bottom-left\n4. Bottom-right\n");
                get_values(state, input, floor, ceil); /* Loop back */
            } else {
                *input -= 1; /*Convert back to index (starting from 0)*/
            }
        }
    } else if (state == EDIT_TRANSLATION) {
        if (ceil != 0 || floor != 0) {
            if (*input < floor || *input > ceil) {
                printf("Please only enter a value between %d and %d\n", floor, ceil);
                get_values(state, input, floor, ceil); /* Loop back */
            }
        } else {
            printf("There should be a scale specified for the inputs!\n");
        }
    }
}

/* Retrieve file path for upload mode */
int get_file(char *input) {
    int status1, status2;
    char line[255];

    if (fgets(line, 255, stdin)) {
        status1 = 1;
    }
    if (sscanf(line, "%s", input)) {
        status2 = 1;
    }
    if (!status1 || !status2) {
        return 1;
    }

    FILE *fp = fopen(input, "r");
    if (fp != NULL) {
        fclose(fp);
    } else {
        printf("File cannot be opened!\nPlease input another:\n");
        get_file(input);
    }

    if (!strstr(input, ".png")) {
        printf("Only PNG files are supported at this time.\nPlease enter a path to a PNG:\n");
        get_file(input);
    }

    return 0;
}

/* Conversion helper - PNG -> PPM */
int pngtoppm(char* filename) {
    char* command = (char*)malloc(sizeof(char)*256);
    char* first_command = "magick ";
    char* second_command = " -compress none ";
    int statusCode = 0;

    strcpy(command, first_command);
    strcat(command, filename);
    strcat(command, second_command);
    strcat(command, "./media/working.ppm");

    if (system(command) != 0) {
        statusCode = 1;
    }
    free(command);

    return statusCode;
}

/* Conversion helper - PPM -> PNG */
int ppmtopng(char* filename) {
    char* command = (char*)malloc(sizeof(char)*256);
    char* first_command = "magick ";
    int statusCode = 0;

    strcpy(command, first_command);
    strcat(command, filename);
    strcat(command, " ");

    strcat(command, "./media/output.png");

    if (system(command) != 0) {
        statusCode = 1;
    }
    free(command);

    return statusCode;
}

/* Construct image struct */
Image *createImage(int width, int height, int max_value) {
    int i, j;
    Image *image = (Image *)malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation for image failed");
        exit(1);
    }

    image->width = width;
    image->height = height;
    image->max_val = max_value;

    /* allocate memory for pixel array */
    image->pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    if (image->pixels == NULL) {
        fprintf(stderr, "Error: Memory allocation for pixel array failed\n");
        exit(1);
    }
    for (i=0; i<height; i++) {
        image->pixels[i] = (Pixel *)malloc(width * sizeof(Pixel));
        if (image->pixels[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation for pixel row %d failed\n", i);
            exit(1);
        } 
        /* If nothing went wrong, set default pixel value to 0 (black) */
        else {
            for (j=0; j<width; j++) {
                image->pixels[i][j].r = 0;
                image->pixels[i][j].g = 0;
                image->pixels[i][j].b = 0;
            }
        }
    }

    return image;
}

/* Frees image */
void freeImage(Image *image) {
    int i;
    for (i = 0; i < image->height; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

/* Main parser - PPM -> Matrix (Pixel**) */
Image* ppmToMatrix(char *filename) {
    char magicHeader[3];
    char line[4096];
    int width, height;
    int maxColor;
    int r, g, b;
    int numLines;
    int expectedNumLines;
    char* token;
    Image* image;
    numLines = 0;
    expectedNumLines = 0;

    /* Open PPM file */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    printf("filename: %s\n", filename);

    /* Read PPM magic header */
    fscanf(file, "%2s", magicHeader);
    magicHeader[2] = '\0';
    printf("magic header: %s\n", magicHeader);

    /* Check PPM file format ("P3" or reject) */
    if (strcmp(magicHeader, "P3") != 0) {
        fprintf(stderr, "Invalid PPM magic header\n");
        exit(1);
    }

    /* Consume new line */
    fgets(line, sizeof(line), file);
    printf("next line after magicHeader: %s", line);

    /* Skip comments and read the next line after # */
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "#", 1) == 0) {
            printf("comment: %s\n", line);
        } else {
            /* Not a comment line, break from the loop */ 
            break;
        }
    }

    /* Read width, height, and maximum color value */
    sscanf(line, "%d %d", &width, &height);
    if (width < 0 || height < 0) {
        fprintf(stderr, "Invalid width or height\n");
        exit(1);
    }
    printf("width: %d, height: %d\n", width, height);

    /* Read maximum color value */
    if (fscanf(file, "%d", &maxColor) != 1 || maxColor < 0 || maxColor > 255) {
        fprintf(stderr, "Invalid maximum color value\n");
        exit(1);
    }
    printf("max color value: %d\n", maxColor);

    /* Consume new line */
    fgets(line, sizeof(line), file);

    /* Calculate number of pixels with width and height */
    expectedNumLines = width * height;
    printf("Expected NumLines: %d\n", expectedNumLines);

    /* Initialize Image structure to store the pixels */
    image = createImage(width, height, maxColor);

    while (fgets(line, sizeof(line), file) != NULL) {
        token = strtok(line, " ");
        while (token != NULL) {

            if (*token == '\n') {
                break;
            }

            /* Parse RGB pixel */ 
            if (sscanf(token, "%d", &r) != 1) {
                fprintf(stderr, "Error: Invalid RGB format in line %d - R\n", numLines);
                exit(1);
            }
            token = strtok(NULL, " ");
            if (sscanf(token, "%d", &g) != 1) {
                fprintf(stderr, "Error: Invalid RGB format in line %d - G\n", numLines);
                exit(1);
            }
            token = strtok(NULL, " ");
            if (sscanf(token, "%d", &b) != 1) {
                fprintf(stderr, "Error: Invalid RGB format in line %d - B\n", numLines);
                exit(1);
            }

            /* Check if RGB values are non-negative */ 
            if (r < 0 || g < 0 || b < 0) {
                fprintf(stderr, "Error: Negative RGB values in line %d\n", numLines);
                exit(1);
            }

            /* Check if RGB values within bit depth range (0-255) */ 
            if (r > 255 || g > 255 || b > 255) {
                fprintf(stderr, "Error: RGB values exceed bit depth range in line %d\n", numLines);
                exit(1);
            }

            /* Convert RGB values to 0 or 255 if using maximal color value (0-1) */ 
            if (maxColor == 1) {
                r = (r == 1) ? 255 : 0;
                g = (g == 1) ? 255 : 0;
                b = (b == 1) ? 255 : 0;
            }

            /* Stores RGB values into Pixel 2D array */
            image->pixels[numLines / width][numLines % width].r = r;
            image->pixels[numLines / width][numLines % width].g = g;
            image->pixels[numLines / width][numLines % width].b = b;
            numLines++;

            token = strtok(NULL, " "); /* Move to the next token */ 
    }

    }
    
    /* Check if number of pixels tally */
    printf("Actual NumLines: %d\n", numLines);
    if (numLines != expectedNumLines) {
        fprintf(stderr, "Number of lines after max color value does not match width and height\n");
        exit(1);
    }

    fclose(file);

    return image;
}


/* Write transformed image into the Netbpm format - Matrix (Pixel**) -> PPM */
char* matrixToPPM(Image *image) {
    int i, j;
    FILE *fptr;
    char* output_file = "./media/output.ppm";

    fptr = fopen(output_file, "w");
    if (!fptr) {
        fprintf(stderr, "Error in opening file.\n");
    }

    /* Write P3 magic header */
    fprintf(fptr, "P3\n");

    /* Write height and width */
    fprintf(fptr, "%d %d\n", image->width, image->height);

    /* Write maximum value */
    fprintf(fptr, "%d\n", image->max_val);

    /* Write out pixels */
    for (i = 0; i < image->height; i++) {
        for (j = 0; j < image->width; j++) {
            fprintf(fptr, "%d %d %d ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);

    return output_file;
}

/* Prints Matrix (Pixel**) - for debugging purposes */
void printMatrix(Image* image) {
    int i, j;
    /* Print the contents of the 2D array */
    for (i = 0; i < image->height; i++) {
        for (j = 0; j < image->width; j++) {
            printf("(%d, %d, %d) ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
        }
        printf("\n");
    }
}