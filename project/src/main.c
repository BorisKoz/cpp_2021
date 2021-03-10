// Copyright 2021 <BKZ>
#include <stdio.h>
#include "../include/cars.h"
#define DBNAME "db.txt"
#define SEARCH_NAME "find.txt"

int main() {
    car* input_car = malloc(sizeof(car));
    car* comparison_car = malloc(sizeof(car));
    car* found_car = malloc(sizeof(car));
    if (input_car == NULL || comparison_car == NULL || found_car == NULL) {
        free(input_car);
        free(comparison_car);
        free(found_car);
        return ALLOCATE_ERROR;
    }
    car_nullptr(input_car);
    car_nullptr(comparison_car);
    car_nullptr(found_car);
    FILE* db = NULL, * search = NULL;
    int return_code = 0;
    return_code = open_car_database(&db, DBNAME)
            + open_car_database(&search, SEARCH_NAME);
    if (return_code != 0) {
        fclose(db);
        fclose(search);
        return return_code;
    }
    float max_equality = 0;
    if (read_car_instance(search, input_car) == 0) {
        while (return_code == 0) {
            return_code = read_car_instance(db, comparison_car);
            if (return_code <= 0) {
                if (max_equality < comparison(input_car, comparison_car)) {
                    max_equality = comparison(input_car, comparison_car);
                    if (copy_car(found_car, comparison_car) == 0) {
                        if (max_equality == 5) {
                            break;
                        }
                    } else {
                        // break of alloc error
                        break;
                    }
                }
            }
            free_car(comparison_car);
        }
    }
    if (return_code <= 0) {
        print_car_instance(found_car);
        return_code = 0;
    }
    free_car(input_car);
    free_car(comparison_car);
    free_car(found_car);
    free(input_car);
    free(comparison_car);
    free(found_car);
    fclose(db);
    fclose(search);

    return return_code;
}
