// Copyright 2021 bkz
#include <sys/types.h>
#include <unistd.h>
#include "../include/search.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void free_all_resources(size_t* divisions, int* processes ) {
    if (divisions) {
        free(divisions);
    }
    if (processes) {
        free(processes);
    }
}

int calculate_process(size_t filesize) {
    if (filesize < 1000) {
        return 1;
    }
    if (filesize < 10000) {
        return 2;
    }
    if (filesize < 100000) {
        return 3;
    }
    if (filesize < 1000000) {
        return 4;
    }
    return MAX_PROCESS;
}

int create_division(int count, size_t filesize, size_t *divisions) {
    size_t proc_range = filesize/count;

    if (divisions == NULL) {
        return NULL_ENTRY;
    }
    for (int i = 1; i < count; i++) {
        divisions[i] = divisions[i-1] + proc_range;
    }
    divisions[count] = filesize;
    return 0;
}

int fork_calculations(int* processes, int process_count) {
    if (processes == NULL) {
        return NULL_ENTRY;
    }
    for (int i=0; i< process_count; i++) {
        int current_id = 0;
        if ((current_id=fork()) == -1) {
            return PID_ERR;
        }
        if (current_id == 0) {
            return i;
        }
        processes[i] = current_id;
    }
    return getpid();
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
    // Предполагается работа с большим файлом, поэтому делим сам файл,
    // а не массив проверяемых символов.
    // calculate processes
    int process_count = calculate_process(file_stat.st_size);

    // fragment array for processes rule
    size_t* divisions = (size_t *)calloc(process_count + 1, sizeof(size_t));
    create_division(process_count, file_stat.st_size, divisions);

    pthread_mutex_t mutex_write_lock;
    pthread_mutex_init(&mutex_write_lock, NULL);

    // setup pipe comms
    int* processes = (int *)calloc(process_count, sizeof(pid_t));
    int pipes[MAX_PROCESS][2];
    memset(pipes, 0, MAX_PROCESS*2);
    for (int i = 0; i < process_count; i++) {
        pipe(pipes[i]);
    }
    // initialise processes
    int current_id = fork_calculations(processes, process_count);

    // this is child process block
    if (getpid() != current_id) {
        write(pipes[current_id][1], "0", 1);
        for (int j = 0; j < size_to_find; j++) {
            int count = 0;
            for (size_t i = divisions[current_id];
                i < divisions[current_id+1]; i++) {
                if (file_in_memory[i] == to_find[j])
                    count++;
            }
            char str[BUFFER_SIZE] = "0";
            snprintf(str, READ_SIZE + 1, FORMAT_STRING, count);
            write(pipes[current_id][1], str, strlen(str));
        }
        char* end_sig = malloc(sizeof(char));
        read(pipes[current_id][0], end_sig, sizeof(char));
        close(pipes[current_id][0]);
        close(pipes[current_id][1]);
        free_all_resources(divisions, processes);
        exit(0);
    }

    // this is parent process block
    for (int i = 0; i < process_count; i++) {
        waitpid(processes[i], NULL, 0);
        for (int j = 0; j < size_to_find; j++) {
            char str[BUFFER_SIZE] = "0";
            read(pipes[i][0], str, READ_SIZE);
            int count = (int)strtol(str, NULL, 10);
            found[j]+= count;
        }
        char buf = 'e';
        write(pipes[i][1], &buf, sizeof(char));
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // free memory
    munmap(file_in_memory, file_stat.st_size);
    free_all_resources(divisions, processes);
    return 0;
}
