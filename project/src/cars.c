// Copyright 2021 <BKZ>

#include "../include/cars.h"
#include <string.h>

// open database file. check if not null.
int open_car_database(FILE** db_ptr, const char* basename) {
    if (!db_ptr) {
        return NULLPTR_EX;
    }
    *db_ptr = fopen(basename, "r+");
    if (*db_ptr == NULL) {
        fprintf(stderr, "Could not open file %s\n", basename);
        return NULLPTR_EX;
    }
    return 0;
}

// read next car instance from base.
int read_car_instance(FILE* db_ptr, car *car_read) {
    if (db_ptr != NULL && car_read != NULL) {
        char read_buffer[5][SIZE_BUF]= {"", "", "", "", ""};
        for (int i = 0; i < 5; i++) {
            if (fscanf(db_ptr, SCAN_FORMAT, read_buffer[i]) != 1) {
                return INCORRECT_ENTRY;
            }
        }
        car_read->engine_power = strtof(read_buffer[0], NULL);
        car_read->maximum_velocity = strtof(read_buffer[1], NULL);
        car_read->fuel_consumption = strtof(read_buffer[2], NULL);
        if (car_read->engine_power <= 0 || car_read->maximum_velocity <= 0
            || car_read->fuel_consumption <= 0) {
            return INCORRECT_ENTRY;
        }
        free_car(car_read);
        car_read->model_name = strdup(read_buffer[3]);
        if (!car_read->model_name) {
            return ALLOCATE_ERROR;
        }
        car_read->body_type = strdup(read_buffer[4]);
        if (!car_read->body_type) {
            free(car_read->model_name);
            return ALLOCATE_ERROR;
        }
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
    // not reached by codecov cuz it prints
    return OUTPUT_ERROR;
}

// float comparison in division. the closer they are,
// the more return approaches 1
float distance_fl(float a, float b) {
    return (a > b ? b / a : a / b);
}

// levenshtein distance in c algorithm
float string_distance(const char* a, const char* b) {
    size_t x = 0, y = 0, len_a = strlen(a), len_b = strlen(b);
    int matrix[SIZE_BUF + 1][SIZE_BUF + 1];
    // matrix initializer
    memset(matrix, 0, sizeof(matrix));
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
    return 1-((float)matrix[len_b][len_a])/len_a;
}

// min_of_3 of 3 for levenshtein
int min(int a, int b) {
    return (a < b ? a : b);
}
int min_of_3(int i, int i1, int i2) {
    return min(i, min(i1, i2));
}

// comparison of 2 cars. returns 5 if completely equal
float comparison(const car* car_1, const car* car_2) {
    float equality = 0;
    equality += distance_fl(car_1->maximum_velocity, car_2->maximum_velocity);
    equality += distance_fl(car_1->engine_power, car_2->engine_power);
    equality += distance_fl(car_1->fuel_consumption, car_2->fuel_consumption);
    equality += string_distance(car_1->model_name, car_2->model_name);
    equality += string_distance(car_1->body_type, car_2->body_type);
    return equality;
}

// free car strings
int free_car(car* car_1) {
    if (car_1 != NULL) {
        if (car_1->body_type) {
            free(car_1->body_type);
            car_1->body_type = NULL;
        }
        if (car_1->model_name) {
            free(car_1->model_name);
            car_1->model_name = NULL;
        }
        return 0;
    }
    return NULLPTR_EX;
}

// copy existing car instance
int copy_car(car* dest, car* src) {
    if (!dest || !src) {
        return NULLPTR_EX;
    }
    free_car(dest);
    dest->fuel_consumption = src->fuel_consumption;
    dest->engine_power = src->engine_power;
    dest->maximum_velocity = src->maximum_velocity;
    dest->model_name = strdup(src->model_name);
    if (!dest->model_name) {
        return ALLOCATE_ERROR;
    }
    dest->body_type = strdup(src->body_type);
    if (!dest->body_type) {
        free(dest->model_name);
        return ALLOCATE_ERROR;
    }
    return 0;
}


int error_out(int err_code) {
    switch (err_code) {
        case 1:
            fprintf(stderr, "%s", "NULL POINTER!");
            return 1;
        case 2:
            fprintf(stderr, "%s", "INCORRECT INPUT");
            return 2;
        case 3:
            fprintf(stderr, "%s", "ALLOCATION FAULT");
            return 3;
        case 4:
            fprintf(stderr, "%s", "INCORRECT OUTPUT");
            return 4;
        default:
            return 0;
    }
}

int search_in_base(car* input_car, car* found_car, FILE* db) {
    int return_code = 0;
    car* comparison_car = (car*)calloc(1, sizeof(car));
    if (comparison_car == NULL) {
        return ALLOCATE_ERROR;
    }
    float max_equality = 0;
    while (return_code == 0) {
        return_code = read_car_instance(db, comparison_car);
        if (return_code > 0) {
            return ALLOCATE_ERROR;
        }
        float current_equality = comparison(input_car, comparison_car);
        if (max_equality < current_equality) {
            max_equality = current_equality;
            if (copy_car(found_car, comparison_car) != 0) {
                return_code = ALLOCATE_ERROR;
                free_car(comparison_car);
                break;
            }
        }
        free_car(comparison_car);
        if (max_equality == 5) {
            break;
        }
    }
    free(comparison_car);
    return return_code;
}


