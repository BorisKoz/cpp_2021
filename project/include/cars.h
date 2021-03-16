// Copyright 2021 <BKZ>

#include <stdio.h>
#include <stdlib.h>

#ifndef PROJECT_CARS_H // NOLINT
#define PROJECT_CARS_H // NOLINT

// define exceptions
#define NULLPTR_EX 1
#define INCORRECT_ENTRY 2
#define ALLOCATE_ERROR 3
#define OUTPUT_ERROR 4
// define finish read signal
#define EOF_REACHED (-1)
#define PARAM_NUMBER 5

// define buffer size
#define SIZE_BUF 40
#define SCAN_FORMAT "%39s"

typedef struct {
    float engine_power;
    float maximum_velocity;
    float fuel_consumption;
    char* body_type;
    char* model_name;
} car;


// open file
int open_car_database(FILE** db_ptr, const char* basename);
int read_car_instance(FILE* db_ptr, car *car_read);
int print_car_instance(const car* car_print);
float comparison(const car* car_1, const car* car_2);
int free_car(car* car_1);
int copy_car(car* dest, car* src);
int search_in_base(car* input_car, car* found_car, FILE* db);
int error_out(int err_code);



#endif // PROJECT_CARS_H // NOLINT
