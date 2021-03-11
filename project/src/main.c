// Copyright 2021 <BKZ>
#include <stdio.h>
#include <stdbool.h>
#include "../include/cars.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return INCORRECT_ENTRY;
    }
    car* input_car = (car*)malloc(sizeof(car));
    car* found_car = (car*)malloc(sizeof(car));

    FILE* db = NULL, * search = NULL;
    int return_code = 0;
    while (true) {
        // проверка аллокации
        if (input_car == NULL  || found_car == NULL) {
            return_code = ALLOCATE_ERROR;
        }
        if (return_code !=0)
            break;
        car_nullptr(input_car);
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
            if (scanf(SCAN_FORMAT ,  read_buffer) != 1) {
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
    if (input_car )
        free(input_car);
    if (found_car)
        free(found_car);
    if (db)
        fclose(db);
    if (search)
        fclose(search);
    return return_code;
}
