// Copyright 2021 bkz

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../include/search.h"
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
    char to_find [BUFFER_SIZE] = "";
    size_t found[BUFFER_SIZE] = {0};
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

    //работа с файлом
    double elapsed[TEST_SERIES_SIZE], average =0;
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
    }
    average = average / TEST_SERIES_SIZE;
    printf("Series average: %lf\n", average);
    if (p) {
        fclose(p);
    }
    return 0;
}
