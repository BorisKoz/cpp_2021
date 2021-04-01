// Copyright 2021 bkz

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../include/search.h"
#include <dlfcn.h>
#define TEST_SERIES_SIZE 3
#define RESULTS_FILE "res.txt"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "No file input");
        return -1;
    }
    // открытие файла
    FILE* p = fopen(argv[1], "r");
    if (!p) {
        fprintf(stderr, "No such file");
        return -1;
    }

    // считывание символов
    int size_to_find = 0;
    char to_find[BUFFER_SIZE] = "";
    size_t found[BUFFER_SIZE];
    memset(found, 0, BUFFER_SIZE * sizeof(*found));
    scanf("%d", &size_to_find);
    if (size_to_find > 30) {
        fprintf(stderr, "Too much symbols");
        fclose(p);
        return -1;
    }
    scanf("%c", &to_find[0]);
    for (int i = 0; i < size_to_find; i++) {
        scanf("%c", &to_find[i]);
    }

    // работа с файлом последовательно
    double elapsed[TEST_SERIES_SIZE], average = 0;
    memset(found, 0, TEST_SERIES_SIZE * sizeof(*elapsed));
    for (int i = 0; i < TEST_SERIES_SIZE; i++) {
        struct timespec start, finish;

        clock_gettime(CLOCK_MONOTONIC, &start);

        file_search(&p, to_find, found, size_to_find);

        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed[i] = (double)(finish.tv_sec - start.tv_sec);
        elapsed[i] += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("elapsed: %lf\n", elapsed[i]);
        average += elapsed[i];
        memset(found, 0, BUFFER_SIZE * sizeof(*found));
    }
    average = average / TEST_SERIES_SIZE;
    printf("Linear series average: %lf\n", average);
    if (p) {
        fclose(p);
    }

    // parallel run
    p = fopen(argv[1], "r");
    void *parallel_lib = dlopen("libbigfile_parallel.dylib", RTLD_LAZY);
    if (!parallel_lib) {
        fprintf(stderr, "LIBRARY NOT FOUND");
        return -1;
    }

    int (*pointer)();
    *(void **)(&pointer) = dlsym(parallel_lib, "file_search");

    double average_parallel = 0;
    memset(found, 0, TEST_SERIES_SIZE * sizeof(*elapsed));
    for (int i = 0; i < TEST_SERIES_SIZE; i++) {
        struct timespec start, finish;

        clock_gettime(CLOCK_MONOTONIC, &start);

        (*pointer)(&p, to_find, found, size_to_find);

        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed[i] = (double)(finish.tv_sec - start.tv_sec);
        elapsed[i] += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("elapsed: %lf\n", elapsed[i]);
        average_parallel += elapsed[i];
        for (int j = 0; j < 2; j++) {
            printf("found %d : %zu \n", j , found[j]);
        }
        memset(found, 0, BUFFER_SIZE * sizeof(*found));
    }
    average_parallel = average_parallel / TEST_SERIES_SIZE;
    printf("Parallel series average: %lf\n", average_parallel);
    dlclose(parallel_lib);

    (average > average_parallel ?
    printf("parallel is faster : %lf vs %lf", average_parallel, average) :
    printf("linear is faster : %lf vs %lf", average, average_parallel));

    if (p) {
        fclose(p);
    }
    return 0;
}
