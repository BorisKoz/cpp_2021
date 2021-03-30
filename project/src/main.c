// Copyright 2021 bkz

#include <stdio.h>

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../include/search.h"

int main() {
    // LC_CTYPE=C tr -dc A-Za-z0-9 < /dev/urandom |
    // fold -w 100 | head -n 100000 > bigfile.txt
    FILE* p = fopen("bigfile.txt", "r");
    char* array = (char*) calloc(3, sizeof(char));
    size_t* found = (size_t*) calloc(3, sizeof(size_t));
    array[0] = 'a';
    array[1] = 'b';
    array[2] = '\n';
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    file_search(&p, array, found, 3);

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (double)(finish.tv_sec - start.tv_sec);
    elapsed += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    for (int i=0; i < 3; i++) {
        printf("%zu ", found[i]);
    }
    printf("\n%lf", elapsed);
    fclose(p);


    free(array);
    free(found);

    return 0;
}
