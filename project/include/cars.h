// Copyright 2021 <BKZ>

#ifndef PROJECT_CARS_H // NOLINT
#define PROJECT_CARS_H // NOLINT

// define exceptions
#define NULLPTR_EX 1
#define INCORRECT_ENTRY 2
#define ALLOCATE_ERROR 3
#define OUTPUT_ERROR 4
// define finish read signal
#define EOF_REACHED -1

#include <stdio.h>
#include <stdlib.h>

#define SIZE_BUF 40

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
int copy_car(car* car_destination, car* src);

int allocate_string(char** string_in_car, const char buffer_value[SIZE_BUF]);
int min_of_3(int i, int i1, int i2);
int car_nullptr(car* car_1);
float string_distance(const char* a, const char* b);
float distance_fl(float a, float b);



#endif // PROJECT_CARS_H // NOLINT
