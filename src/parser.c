/* Parser of all user inputs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "fsm.h"
/* MICHELLE */

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

/* Handle any numerical input for edit commands here - angles/degrees whatever */

/* Our bitmap parser here huehue*/
/* output shd be a 2D array with each pointer pointing to each row's pixels*/

Image *createImage(int width, int height) {
    Image *image = (Image *)malloc(sizeof(Image));
    int i;

    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation for image failed");
        exit(1);
    }

    image->width = width;
    image->height = height;

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
    }

    return image;
}

void freeImage(Image *image) {
    int i;
    for (i = 0; i < image->height; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

void ppmToMatrix(const char *filename) {
    FILE *file = fopen(filename, "r");
    int width, height, maxColor, numLines, expectedNumLines, r, g, b, i, j;
    /* initialize Image structure to store the pixels */
    Image *image = createImage(width, height);
    char magicHeader[3];
    char nextLine[256];
    char line[256];
    char nextLine2[256];

    /* open PPM file */
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    printf("filename: %s\n", filename);

    /* read PPM magic header */
    fscanf(file, "%2s", magicHeader); /* read from file */
    magicHeader[2] = '\0';
    printf("magic header: %s\n", magicHeader);

    /* check PPM file format ("P3" or reject) */
    if (strcmp(magicHeader, "P3") != 0) {
    /* magicHeader[0] != 'P' || magicHeader[1] != '3' */
        fprintf(stderr, "Invalid PPM magic header\n");
        exit(1);
    }

    /* somehow needs this to go to read the comments i'm not sure why */
    fgets(nextLine, sizeof(nextLine), file);
    printf("next line after magicHeader: %s", nextLine);

    /* Skip comments and read the next line after # */
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "#", 1) == 0) {
            /* Comment line, skip it */ 
            /* printf("comment line: %s", line); */
        } else {
            /* Not a comment line, break from the loop */ 
            break;
        }
    }

    /* read width, height, and maximum color value */
    sscanf(line, "%d %d", &width, &height); /* read by line when skipping comments */
    if (width < 0 || height < 0) {
        fprintf(stderr, "Invalid width or height\n");
        /* shd i exit? If exit, can't read the max color, but if i don't exit, it will go to the next ones (matrix related)*/
        exit(1);
    }
    printf("width: %d, height: %d\n", width, height);

    /* read maximum color value */
    if (fscanf(file, "%d", &maxColor) != 1 || maxColor < 0 || maxColor > 255) {
        fprintf(stderr, "Invalid maximum color value\n");
        exit(1);
    }
    printf("max color value: %d\n", maxColor);

    /* SOMEHOW HAVE AN EMPTHY LINE AFTER MAX COLOR VALUE */
    fgets(nextLine2, sizeof(nextLine2), file);
    printf("next line after max color value: %s", nextLine2);

    /* check if number of lines tally with width and height values */
    numLines = 0;
    expectedNumLines = width * height;
    printf("Expected NumLines: %d\n", expectedNumLines);

    while (fgets(line, sizeof(line), file) != NULL) {
        /* printf("read lines: %s", line); */

        /* check if line contains valid RGB triplets not exceeding 70 characters */ 
        if (strlen(line) > 70) {
            fprintf(stderr, "Error: Line %d contains more than 70 characters\n", numLines);
            exit(1);
        }

        /* parse RGB pixel */
        if (sscanf(line, "%d %d %d", &r, &g, &b) != 3) {
            fprintf(stderr, "Error: Invalid RGB format in line %d\n", numLines);
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
        /* printf("\current number of lines: %d\n", numLines); */

    }
    
    printf("Actual NumLines: %d\n", numLines);

    /* print the contents of the 2D array */
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            printf("(%d, %d, %d) ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
        }
        printf("\n");
    }

    if (numLines != expectedNumLines) {
        fprintf(stderr, "Number of lines after max color value does not match width and height\n");
        exit(1);
    }


    freeImage(image);

    fclose(file);
}

/* HAVEN'T USED YET */
void printImage(Image *image) {
    int i, j;
    for (i = 0; i < image->height; i++) {
        printf("Row%d* = [", i + 1);

        for (j = 0; j < image->width; j++) {
            printf("(%d, %d, %d)", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);

            /*Add a comma and space for all elements except the last one*/
            if (j < image->width - 1) {
                printf(", ");
            }
        }

        printf("]\n");
    }
}
/*
int main() {
    const char *filename = "example.ppm";

    ppmToMatrix(filename);

    return 0;
}*/