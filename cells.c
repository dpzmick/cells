// I am a small wolfram elementary cellular automata simulator
// David Zmick wrote this, but he really doesn't care what you do with it.
// This is public domain

// OpenMP Version

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>

/*******************************************************************************/
/* Image Handling                                                              */
/*******************************************************************************/
// sets up an image file to write to
// pgm with max gray set at 1. Doing this to avoid diddling around with bits.
FILE* make_pgm(char name[], int length, int timesteps) {
    FILE *fp = fopen(name, "wb");
    fprintf(fp, "P5\n %d\n %d\n %d\n", length, timesteps, 1);
    return fp;
}

// write a line to an image file
void write_line(char *line, int line_length, FILE* fp) {
    for (int i = 0; i < line_length; i++) {
        fprintf(fp, "%d", line[i]);
        if (i != line_length - 1) { fprintf(fp, " "); }
    }
    fprintf(fp, "\n");
    return;
}


/*******************************************************************************/
/* Rules                                                                       */
/*******************************************************************************/
// Applies specified rule to input and writes result to output
// assumes anything "off the map" is dead
// output must be a chunk of memory we can overwrite of the right size
void rule(int rule, char* input, int length, char* output) {
    int left, right, above, left_i, right_i;
    #pragma omp for
    for (int i = 0; i < length; i++) {
        left_i = i - 1;
        right_i = i + 1;
        if (left_i < 0) { left = 0; }
        else { left = input[left_i]; }
        if (right_i > length - 1) { right = 0; }
        else { right = input[right_i]; }
        above = input[i];
        
        switch(left*100 + above*10 + right){
            case 111: output[i] = (rule >> 7) & 1; break;
            case 110: output[i] = (rule >> 6) & 1; break;
            case 101: output[i] = (rule >> 5) & 1; break;
            case 100: output[i] = (rule >> 4) & 1; break;
            case  11: output[i] = (rule >> 3) & 1; break;
            case  10: output[i] = (rule >> 2) & 1; break;
            case   1: output[i] = (rule >> 1) & 1; break;
            case   0: output[i] = (rule >> 0) & 1; break;
            default:  output[i] = 0;break;
        }
    }
    return;
}

/*******************************************************************************/
/* Initialization                                                              */
/*******************************************************************************/
char* random_init(int length) {
    char *init = (char*) malloc(length * sizeof(char));
    for (int i = 0; i < length; i++) {
        init[i] = (rand() % 100) > 50;
    }
    return init;
}

char* centered_init(int length) {
    char *init = (char*) calloc(length, sizeof(char));
    init[length / 2] = 1;
    return init;
}

/*******************************************************************************/
/* Main                                                                        */
/*******************************************************************************/
void usage() {
    printf("Usage: cells r l t\n");
    printf("r - base-10 rule to use\n");
    printf("l - length of row\n");
    printf("t - timesteps to simulate\n");
    exit(0);
}
int main(int argc, char **argv) {
    if (argc != 4) { usage(); }
    srand(time(NULL));

    int rule_no = atoi(argv[1]);
    int length = atoi(argv[2]);
    int timesteps = atoi(argv[3]);
    printf("Cells r:%d, l:%d, t:%d\n", rule_no, length, timesteps);
    FILE* fp = make_pgm("output.pbm", length, timesteps);

    // TODO read initial state from file
    //printf("Generating input\n");
    //char *data = random_init(length);
    char *data = centered_init(length);

    //printf("Generating output\n");
    char *output = (char*) malloc(length * sizeof(char*));

    //printf("Running simulation\n");
    #pragma omp parallel
    for (int t = 0; t < timesteps; t++) {
        #pragma omp single
        {
        fwrite(data, sizeof(char), length, fp);
        rule(rule_no, data, length, output);
        memcpy(data, output, length * sizeof(char));
        }
    }

    fflush(fp);
    fclose(fp);
    free(data);
    free(output);
    exit(0);
}
