// Copyright 2021 <BKZ>
#include <stdio.h>
#include <stdbool.h>
#include "cars.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        return INCORRECT_ENTRY;
    }
    car* input_car = (car*)calloc(1, sizeof(car));
    if (!input_car) {
        return  ALLOCATE_ERROR;
    }
    car* found_car = (car*)calloc(1, sizeof(car));
    if (!found_car) {
        free(input_car);
        return ALLOCATE_ERROR;
    }
    FILE* db = NULL, * search = NULL;
    int return_code = 0;
    while (true) {
        // open fault check
        if (open_car_database(&db, argv[1]) != 0 ||
            open_car_database(&search, argv[2]) != 0) {
            error_out(ALLOCATE_ERROR);
            break;
        }

        // write to file for search
        char read_buffer[SIZE_BUF] = {""};
        for (int i = 0; i < PARAM_NUMBER; i++) {
            if (scanf(SCAN_FORMAT ,  read_buffer) != 1) {
               error_out(INCORRECT_ENTRY);
               break;
            } else {
                fputs(read_buffer, search);
                if (i < PARAM_NUMBER - 1) {
                    fputs(" ", search);
                }
            }
        }
        rewind(search);

        // search itself
        return_code = read_car_instance(search, input_car);
        if (return_code > 0)
            break;
        return_code = search_in_base(input_car, found_car, db);
        if (return_code <= 0) {
            print_car_instance(found_car);
            return_code = 0;
        }
        break;
    }
    // free all the memory
    error_out(return_code);
    free_car(found_car);
    free_car(input_car);
    free(input_car);
    free(found_car);
    if (db) {
        fclose(db);
    }
    if (search)  {
        fclose(search);
    }
    return return_code;
}
