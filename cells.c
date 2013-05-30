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
int* rule30(int* input, int length) {
    int *output = (int*) malloc(length * sizeof(int));
    int left, right, above, left_i, right_i;
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        left_i = i - 1;
        right_i = i + 1;
        if (left_i < 0) { left = 0; }
        else { left = input[left_i]; }
        if (right_i > length - 1) { right = 0; }
        else { right = input[right_i]; }
        above = input[i];

        if (       ( left && !above && !right)
                || (!left &&  above &&  right)
                || (!left &&  above && !right)
                || (!left && !above &&  right)) {
            output[i] = 1;
        } else {
            output[i] = 0;
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
int main(int argc, char **argv) {
    srand(time(NULL));

    int length = 10000;
    int timesteps = 5000;
    FILE* fp = make_pbm("output.pbm", length, timesteps);

    printf("Generating input\n");
    //int *init = random_init(length);
    int *data = centered_init(length);

    printf("Running simulation\n");

    for (int t = 1; t < timesteps; t++) {
        write_line(data, length, fp);
        data = rule30(data, length);
    }

    write_line(data, length, fp);
    fflush(fp);
    fclose(fp);
    free(data);

    exit(0);
}
