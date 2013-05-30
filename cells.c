// I am a small wolfram elementary cellular automata simulator
// David Zmick wrote this, but he really doesn't care what you do with it.
// This is public domain

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*******************************************************************************/
/* Image Handling                                                              */
/*******************************************************************************/
// sets up an image file to write to
FILE* make_pbm(char name[], int length, int timesteps) {
    FILE *fp = fopen(name, "w");
    fprintf(fp, "P1\n");
    fprintf(fp, "%d %d\n", length, timesteps);
    return fp;
}

// write a line to an image file
void write_line(int *line, int line_length, FILE* fp) {
    for (int i = 0; i < line_length; i++) {
        fprintf(fp, "%d", line[i]);
        if (i != line_length - 1) { fprintf(fp, " "); }
    }
    fprintf(fp, "\n");
}


/*******************************************************************************/
/* Rules                                                                       */
/*******************************************************************************/
// Builds new array and returns a pointer to it following rule 30.
// assumes anything "off the map" is dead

int* rule(int rule, int* input, int length) {
    int *output = (int*) malloc(length * sizeof(int));
    int left, right, above, left_i, right_i;
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        // TODO make in place
        left_i = i - 1;
        right_i = i + 1;
        if (left_i < 0) { left = 0; }
        else { left = input[left_i]; }
        if (right_i > length - 1) { right = 0; }
        else { right = input[right_i]; }
        above = input[i];
    
        printf("%d%d%d%d%d%d%d%d\n",
                (rule >> 7) & 1,
                (rule >> 6) & 1,
                (rule >> 5) & 1,
                (rule >> 4) & 1,
                (rule >> 3) & 1,
                (rule >> 2) & 1,
                (rule >> 1) & 1,
                (rule >> 0) & 1);


        int repr = left*100 + above*10 + right;
        printf("%d\n", repr);
        switch(left*100 + above*10 + right){
            case 111:
                output[i] = (rule >> 7) & 1;
                break;
            case 110:
                output[i] = (rule >> 6) & 1;
                break;
            case 101:
                output[i] = (rule >> 5) & 1;
                break;
            case 100:
                output[i] = (rule >> 4) & 1;
                break;
            case 11: //011
                output[i] = (rule >> 3) & 1;
                break;
            case 10: //010
                output[i] = (rule >> 2) & 1;
                break;
            case 1: //001
                output[i] = (rule >> 1) & 1;
                break;
            case 0:
                output[i] = (rule >> 0) & 1;
                break;
        }
    }
    return output;
}

/*******************************************************************************/
/* Initialization                                                              */
/*******************************************************************************/
int* random_init(int length) {
    int *init = (int*) malloc(length * sizeof(int));
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        init[i] = (rand() % 100) > 50;
    }
    return init;
}

int* centered_init(int length) {
    int *init = (int*) calloc(length, sizeof(int));
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
    FILE* fp = make_pbm("output.pbm", length, timesteps);

    // TODO read initial state from file
    printf("Generating input\n");
    //int *init = random_init(length);
    int *data = centered_init(length);

    printf("Running simulation\n");

    for (int t = 0; t < timesteps; t++) {
        write_line(data, length, fp);
        data = rule(rule_no, data, length);
    }

    fflush(fp);
    fclose(fp);
    free(data);

    exit(0);
}
