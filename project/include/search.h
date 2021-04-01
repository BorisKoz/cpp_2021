// Copyright 2021 bkz
#ifndef PROJECT_INCLUDE_SEARCH_H_
#define PROJECT_INCLUDE_SEARCH_H_
#include <stdio.h>

#define NULL_ENTRY (-1)
#define MMAP_ERR 2
#define PID_ERR 3
#define WRONG_OPEN 4
#define MAX_PROCESS 10
#define READ_SIZE 20
#define BUFFER_SIZE 30
#define FORMAT_STRING "%020d"

int file_search(FILE** , const char* , size_t* , size_t size_to_find);
#endif  // PROJECT_INCLUDE_SEARCH_H_
