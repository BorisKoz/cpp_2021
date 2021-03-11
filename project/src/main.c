// Copyright 2021 <BKZ>
#include <stdio.h>
#include <stdbool.h>
#include "../include/cars.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return INCORRECT_ENTRY;
    }
    car* input_car = (car*)malloc(sizeof(car)); //NOLINT
    car* comparison_car = (car*)malloc(sizeof(car)); //NOLINT
    car* found_car = (car*)malloc(sizeof(car)); //NOLINT
    FILE* db = NULL, * search = NULL;
    int return_code = 0;
    while (true) {
        // Allocation fault check
        if (input_car == NULL || comparison_car == NULL || found_car == NULL) {
            return_code = ALLOCATE_ERROR;
        }
        if (return_code !=0)
            break;
        car_nullptr(input_car);
        car_nullptr(comparison_car);
        car_nullptr(found_car);

        // open fault check
        if (open_car_database(&db, argv[1]) != 0 ||
            open_car_database(&search, argv[2]) != 0) {
            return_code = ALLOCATE_ERROR;
        }
        if (return_code !=0)
            break;

        // write to file for search
        char read_buffer[SIZE_BUF];
        for (int i = 0; i < 5; i++) {
            if (scanf("%35s", read_buffer) != 1) {
               return_code = INCORRECT_ENTRY;
            } else {
                fputs(read_buffer, search);
                if (i < 4)
                    fputs(" ", search);
            }
        }
        rewind(search);
        if (return_code !=0)
            break;
        // search itself
        float max_equality = 0;
        if (read_car_instance(search, input_car) <= 0) {
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
            }
        }
        if (return_code <= 0) {
            print_car_instance(found_car);
            return_code = 0;
        }
        break;
    }
    free_car(found_car);
    free_car(comparison_car);
    free_car(input_car);
    if (input_car != NULL)
        free(input_car);
    if (comparison_car != NULL)
        free(comparison_car);
    if (found_car != NULL)
        free(found_car);
    if (db != NULL)
        fclose(db);
    if (search != NULL)
        fclose(search);
    return return_code;
}
