#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Builds new array and returns a pointer to it following rule 30.
// assumes anything "off the map" is dead
int* rule30(int* input, int length) {
    int *output = (int*) malloc(length * sizeof(int));
    int left, right, above, left_i, right_i;
    #pragma opm parallel for
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

int* random_init(int length) {
    int *init = (int*) malloc(length * sizeof(int));
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

int main(int argc, char **argv) {
    int length = 100000;
    int timesteps = 5000000;

    srand(time(NULL));
    //int *init = random_init(length);
    int *init = centered_init(length);

    int **data = (int**) malloc(timesteps * sizeof(int*));
    
    data[0] = init;
    for (int t = 1; t < timesteps; t++) {
        data[t] = rule30(data[t-1], length);
    }

    // TODO need pretty output, generate bitmap?
    // TODO find a way to use less memory.

    exit(0);
}
