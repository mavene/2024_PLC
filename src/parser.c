/* Parser of all user inputs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "fsm.h"
/* MICHELLE */

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
            get_command(state, input); /* Loop back */
        } 
        else if (state == PREVIEW_MODE && (*input != 'Y' && *input != 'N')) {
            printf("Please enter only Y (Yes) or N (No)\n");
            get_command(state, input); /* Loop back */
        }
        else if (state == EDIT_MODE && (*input != '1' && 
                                        *input != '2' && 
                                        *input != '3' &&
                                        *input != '4' &&
                                        *input != '5')) {
            printf("Please enter only these options below\n1. Translation (Crop)\n2. Scale (Zoom)\n3. Rotation\n4. Edge Detection\n5.Preview Mode\n");
            get_command(state, input); /* Loop back */
        }
    }
}


/* Retrieve file path */
// char* get_path(State state, char* input) {
//     char line[256] = {};
//     sscanf(line, "%s", input);
//     if (line != EOF && access(line, F_OK)) {
//         strip_ext(line);
//     } else {
//         printf("Please enter an existing file path\n");
//         get_path(state, input); /* Loop back */
//     }
//     return line;
// }

/* Handle any numerical input for edit commands here - angles/degrees whatever */

void strip_ext(char *fname) {
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.' && *end != '\\' && *end != '/') {
        --end;
    }
    if ((end > fname && *end == '.') &&
        (*(end - 1) != '\\' && *(end - 1) != '/')) {
        *end = '\0';
    }  
}

/* Conversion helpers */
int pngtoppm(char* filename) {
    char* command = (char*)malloc(sizeof(char)*256);
    char* first_command = "magick ";
    char* second_command = " -compress none ";
    //int status;

    strcpy(command, first_command);
    strcat(command, filename);
    strcat(command, second_command);
    strcat(command, "example.ppm");

    system(command); // status
    free(command);

    return 0; //status;
}

int ppmtopng(char* filename) {
    char* command = (char*)malloc(sizeof(char)*256);
    //char* filename_copy = strdup(filename);
    char* first_command = "magick ";
    //int status;

    strcpy(command, first_command);
    strcat(command, filename);
    strcat(command, " ");

    //strip_ext(filename_copy);
    // strcat(command, filename_copy);
    strcat(command, "output.png"); //.png only

    system(command); // status
    free(command);

    return 0; //status;
}

/* Output should be a struct containing a 2D array of pointers to every row's pixels*/
Image *createImage(int width, int height, int max_value, char* filename) {
    Image *image = (Image *)malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation for image failed");
        exit(1);
    }

    image->width = width;
    image->height = height;
    image->max_val = max_value;
    image->filename = filename;

    /* allocate memory for pixel array */
    image->pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    if (image->pixels == NULL) {
        fprintf(stderr, "Error: Memory allocation for pixel array failed\n");
        exit(1);
    }
    for (int i=0; i<height; i++) {
        image->pixels[i] = (Pixel *)malloc(width * sizeof(Pixel));
        if (image->pixels[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation for pixel row %d failed\n", i);
            exit(1);
        } 
        /* If nothing went wrong, set default pixel value to 0 (black) */
        else {
            for (int j=0; j<width; j++) {
                image->pixels[i][j].r = 0;
                image->pixels[i][j].g = 0;
                image->pixels[i][j].b = 0;
            }
        }
    }

    return image;
}

void freeImage(Image *image) {
    for (int i = 0; i < image->height; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

Image* ppmToMatrix(char *filename) {
    /* open PPM file */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    printf("filename: %s\n", filename);

    /* read PPM magic header */
    char magicHeader[3];
    fscanf(file, "%2s", magicHeader); /* read from file */
    magicHeader[2] = '\0';
    printf("magic header: %s\n", magicHeader);

    /* check PPM file format ("P3" or reject) */
    if (strcmp(magicHeader, "P3") != 0) {
    /* magicHeader[0] != 'P' || magicHeader[1] != '3' */
        fprintf(stderr, "Invalid PPM magic header\n");
        exit(1);
    }

    /* consume new line */
    char line[4096];
    fgets(line, sizeof(line), file);
    printf("next line after magicHeader: %s", line);

    /* skip comments and read the next line after # */
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "#", 1) == 0) {
            printf("comment: %s\n", line);
        } else {
            /* Not a comment line, break from the loop */ 
            break;
        }
    }

    /* read width, height, and maximum color value */
    int width, height;
    sscanf(line, "%d %d", &width, &height); /* read by line when skipping comments */
    if (width < 0 || height < 0) {
        fprintf(stderr, "Invalid width or height\n");
        exit(1);
    }
    printf("width: %d, height: %d\n", width, height);

    /* read maximum color value */
    int maxColor;
    if (fscanf(file, "%d", &maxColor) != 1 || maxColor < 0 || maxColor > 255) {
        fprintf(stderr, "Invalid maximum color value\n");
        exit(1);
    }
    printf("max color value: %d\n", maxColor);

    /* consume new line */
    fgets(line, sizeof(line), file);

    /* check if number of lines tally with width and height values */
    int numLines = 0;
    int expectedNumLines = width * height;
    printf("Expected NumLines: %d\n", expectedNumLines);

    /* initialize Image structure to store the pixels */
    Image *image = createImage(width, height, maxColor, filename);

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, " ");
        while (token != NULL) {

            if (*token == '\n') {
                break;
            }

            int r, g, b;
            /* parse RGB pixel */ 
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

            /* check if RGB values are non-negative */ 
            if (r < 0 || g < 0 || b < 0) {
                fprintf(stderr, "Error: Negative RGB values in line %d\n", numLines);
                exit(1);
            }

            /* check if RGB values within bit depth range (0-255) */ 
            if (r > 255 || g > 255 || b > 255) {
                fprintf(stderr, "Error: RGB values exceed bit depth range in line %d\n", numLines);
                exit(1);
            }

            /* convert RGB values to 0 or 255 if using maximal color value (0-1) */ 
            if (maxColor == 1) {
                r = (r == 1) ? 255 : 0;
                g = (g == 1) ? 255 : 0;
                b = (b == 1) ? 255 : 0;
            }

            /* stores RGB values in the 2D array */
            image->pixels[numLines / width][numLines % width].r = r;
            image->pixels[numLines / width][numLines % width].g = g;
            image->pixels[numLines / width][numLines % width].b = b;
            numLines++;

            /* printf("current number of lines: %d\n", numLines); */

            token = strtok(NULL, " "); /* move to the next token */ 
    }

    }
    
    printf("Actual NumLines: %d\n", numLines);

    if (numLines != expectedNumLines) {
        fprintf(stderr, "Number of lines after max color value does not match width and height\n");
        exit(1);
    }

    fclose(file);

    return image;
}

char* matrixToPPM(Image *image) {
    //int i, j;
    FILE *fptr;
    char* output_file = "output.ppm"; // change to filename from image

    fptr = fopen(output_file, "w");
    if (!fptr) {
        fprintf(stderr, "Error in opening file.\n");
    }

    /*Write P3 magic header*/
    fprintf(fptr, "P3\n");

    /*Write height and width*/
    fprintf(fptr, "%d %d\n", image->width, image->height);

    /*Write maximum value*/
    fprintf(fptr, "%d\n", image->max_val); /*change to accomodate TODO above*/

    /*Write out pixels*/
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            fprintf(fptr, "%d %d %d ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);

    return output_file;
}

void printMatrix(Image* image) {
    /* print the contents of the 2D array */
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            printf("(%d, %d, %d) ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
        }
        printf("\n");
    }
}

// int main(int argc, char ** argv) {
//     const char *filename = argv[1];

//     ppmToMatrix(filename);

//     return 0;
// }