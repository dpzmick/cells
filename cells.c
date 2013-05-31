// I am a small wolfram elementary cellular automata simulator
// David Zmick wrote this, but he really doesn't care what you do with it.
// This is public domain

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
void apply(int rule, char* input, int length, char* output) {
    int left, right, above, left_i, right_i;
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
char* centered_init(int length) {
    char *init = (char*) calloc(length, sizeof(char));
    init[length / 2] = 1;
    return init;
}
/*******************************************************************************/
/* Rule                                                                        */
/*******************************************************************************/

void rule(int rule_no, int length, int timesteps) {
    char filename[20];
    sprintf(filename, "output/rule%03d.pbm", rule_no);
    printf("making %s\n", filename);
    FILE* fp = make_pgm(filename, length, timesteps);
    char *data = centered_init(length);
    char *output = (char*) malloc(length * sizeof(char*));
    for (int t = 0; t < timesteps; t++) {
        fwrite(data, sizeof(char), length, fp);
        apply(rule_no, data, length, output);
        memcpy(data, output, length * sizeof(char));
    }
    fflush(fp);
    fclose(fp);
    free(data);
    free(output);
}
/*******************************************************************************/
/* Main                                                                        */
/*******************************************************************************/
void usage() {
    printf("Usage: cells r l t\n");
    printf("l - length of row\n");
    printf("t - timesteps to simulate\n");
    exit(0);
}
int main(int argc, char **argv) {
    if (argc != 3) { usage(); }
    int length = atoi(argv[1]);
    int timesteps = atoi(argv[2]);
    
    #pragma omp parallel for
    for (int rule_no = 0; rule_no <= 255; rule_no++) {
        rule(rule_no, length, timesteps);
    }
    return 0;
}
