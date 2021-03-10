// Copyright 2021 <BKZ>

#include "../include/cars.h"
#include <string.h>

// open database file. check if not null.
int open_car_database(FILE** db_ptr, const char* basename) {
    *db_ptr = fopen(basename, "r");
    if (*db_ptr == NULL) {
        printf("Could not open file %s\n", basename);
        return NULLPTR_EX;
    }
    return 0;
}

int allocate_string(char** string_in_car, const char buffer_value[SIZE_BUF]) {
    *string_in_car = (char*)malloc((strlen(buffer_value) + 1) * sizeof(char)); //NOLINT
    if (*string_in_car != NULL) {
        if (snprintf(*string_in_car, SIZE_BUF, "%s", buffer_value) < 0) {
            free(*string_in_car);
            return INCORRECT_ENTRY;
        }
        return 0;
    }
    return ALLOCATE_ERROR;
}

// read next car instance from base.
int read_car_instance(FILE* db_ptr, car *car_read) {
    if (db_ptr != NULL && car_read != NULL) {
        char read_buffer[5][SIZE_BUF];
        if (fscanf(db_ptr, "%35s%35s%35s%35s%35s", read_buffer[0],
                   read_buffer[1], read_buffer[2], read_buffer[3],
                   read_buffer[4]) !=5 ) {
            return INCORRECT_ENTRY;
        }
        if (strtof(read_buffer[0], NULL) > 0 && strtof(read_buffer[1], NULL) > 0
            && strtof(read_buffer[2], NULL) > 0) {
            car_read->engine_power = strtof(read_buffer[0], NULL);
            car_read->maximum_velocity = strtof(read_buffer[1], NULL);
            car_read->fuel_consumption = strtof(read_buffer[2], NULL);
        }

        if (allocate_string(&car_read->model_name, read_buffer[3]) != 0
            || allocate_string(&car_read->body_type, read_buffer[4]) != 0)
            return ALLOCATE_ERROR;
        if (feof(db_ptr)) {
            return EOF_REACHED;
        }
        return 0;
    }
    return NULLPTR_EX;
}

// output car instance
int print_car_instance(const car* car_print) {
    if (printf ("EP:%f  V:%f FC%f Model:%s BT:%s \n", car_print->engine_power,
                car_print->maximum_velocity, car_print->fuel_consumption,
                car_print->model_name, car_print->body_type) > 0)
        return 0;
    return OUTPUT_ERROR;
}

// float comparison in division. the closer they are,
// the more return approaches 0
float distance(float a, float b) {
    if (a > b)
        return b/a;
    else
        return a/b;
}

// levenshtein distance in c algorithm
float string_distance(const char* a, const char* b) {
    size_t x, y, len_a, len_b;
    len_a = strlen(a);
    len_b = strlen(b);
    int matrix[len_b + 1][len_a + 1];
    // matrix initializer
    for (x = 0; x <= len_b; x++) {
        for (y = 0; y <= len_a; y++) {
            matrix[x][y] = 0;
        }
    }
    matrix[0][0] = 0;
    for (x = 1; x <= len_b; x++)
        matrix[x][0] = matrix[x-1][0]+1;
    for (y = 1; y <= len_a; y++)
        matrix[0][y] = matrix[0][y]+1;
    for (x = 1; x <= len_b; x++)
        for (y = 1; y <= len_a; y++)
            matrix[x][y] = min_of_3(matrix[x - 1][y] + 1, matrix[x][y - 1] + 1,
                                    matrix[x - 1][y - 1] +
                                    (a[y - 1] == b[x - 1] ? 0 : 1));
    return 1-((float)matrix[len_b][len_a])/len_a; //NOLINT
}

// min_of_3 of 3 for levenshtein
int min_of_3(int i, int i1, int i2) {
    if (i < i1) {
        if (i < i2) {
            return i;
        } else {
            return i2;
        }
    }
    if (i2 < i1) {
        return i2;
    }
    return i1;
}

// comparison of 2 cars. returns 5 if completely equal
float comparison(const car* car_1, const car* car_2) {
    float equality = 0;
    equality += distance(car_1->maximum_velocity, car_2->maximum_velocity);
    equality += distance(car_1->engine_power, car_2->engine_power);
    equality += distance(car_1->fuel_consumption, car_2->fuel_consumption);
    equality += string_distance(car_1->model_name, car_2->model_name);
    equality += string_distance(car_1->body_type, car_2->body_type);
    return equality;
}

// free car strings
int free_car(car* car_1) {
    if (car_1->body_type != NULL )
        free(car_1->body_type);
    if (car_1->model_name != NULL )
        free(car_1->model_name);
    return 0;
}

// copy existing car instance
int copy_car(car* dest, car* src) {
    free_car(dest);
    if (dest == NULL)
        return ALLOCATE_ERROR;
    dest->fuel_consumption = src->fuel_consumption;
    dest->engine_power = src->engine_power;
    dest->maximum_velocity = src->maximum_velocity;
    car_nullptr(dest);
    allocate_string(&dest->model_name, src->model_name);
    allocate_string(&dest->body_type, src->body_type);
    return 0;
}
int car_nullptr(car* car_1) {
    car_1->body_type = NULL;
    car_1->model_name = NULL;
    return 0;
}
