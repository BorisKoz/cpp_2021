// Copyright 2021 bkz

#include <sys/stat.h>
#include <sys/mman.h>
#include "../include/search.h"

int compare_to_array(const char* to_compare, char buffer, size_t size_to_find) {
    for (int i = 0; i < size_to_find; i++) {
        if (to_compare[i] == buffer) {
            return i;
        }
    }
    return -1;
}

int file_search(FILE** fp, const char* to_find,
                size_t* found, size_t size_to_find) {
    if (fp == NULL || *fp == NULL || to_find == NULL || found == NULL) {
        return NULL_ENTRY;
    }
    struct stat file_stat;
    fstat(fileno(*fp), &file_stat);
    if ((file_stat.st_mode & S_IRUSR) == 0) {
        return WRONG_OPEN;
    }
    if (file_stat.st_size == 0) {
        return NULL_ENTRY;
    }
    char* file_in_memory = (char *)mmap(NULL, file_stat.st_size,
                                        PROT_READ, MAP_SHARED, fileno(*fp), 0);
    if (file_in_memory == NULL) {
        return MMAP_ERR;
    }
    for (size_t i = 0; i < file_stat.st_size; i++) {
        int position = compare_to_array(to_find,
                                        file_in_memory[i], size_to_find);
        if (position != -1) {
            found[position]++;
        }
    }
    munmap(file_in_memory, file_stat.st_size);
    return 0;
}
