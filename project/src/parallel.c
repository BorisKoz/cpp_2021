// Copyright 2021 bkz
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/search.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#define unlikely(expr) __builtin_expect(!!(expr), 0)

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
    return (MAX_PROCESS > (int)sysconf(_SC_NPROCESSORS_ONLN) ?
            (int)sysconf(_SC_NPROCESSORS_ONLN) : MAX_PROCESS);
}

int create_division(
        int count, size_t filesize, size_t *divisions) {
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

int child_process_run(const char* file_in_memory, const size_t* divisions,
                       const int current_id, const int size_to_find,
                       const char* to_find, int pipes[MAX_PROCESS][2]) {
    for (int j = 0; j < size_to_find; j++) {
        int count = 0;
        for (size_t i = divisions[current_id];
             i < divisions[current_id+1]; i++) {
            if (unlikely(file_in_memory[i] == to_find[j]))
                count++;
        }
        char str[BUFFER_SIZE] = "0";
        snprintf(str, READ_SIZE + 1, FORMAT_STRING, count);
        write(pipes[current_id][1], str, strlen(str));
    }
    close(pipes[current_id][0]);
    close(pipes[current_id][1]);
    return 0;
}

int file_search(FILE** fp, const char* to_find,
                         size_t* found, size_t size_to_find) {
    // correct checks
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
        child_process_run(file_in_memory, divisions, current_id,
                          size_to_find, to_find, pipes);
        free_all_resources(divisions, processes);
        fclose(*fp);
        exit(0);
    }

    // this is parent process block
    for (int i = 0; i < process_count; i++) {
        while (!waitpid(processes[i], NULL, 0)) {
        }
        for (int j = 0; j < size_to_find; j++) {
            char str[BUFFER_SIZE] = "0";
            read(pipes[i][0], str, READ_SIZE);
            int count = (int)strtol(str, NULL, 10);
            found[j]+= count;
        }
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // free memory
    munmap(file_in_memory, file_stat.st_size);
    free_all_resources(divisions, processes);
    return 0;
}
