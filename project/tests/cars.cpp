// Copyright 2021 <BKZ>

#include "gtest/gtest.h"

extern "C" {
#include "./cars.h"
}

// tests for in-program logic
TEST(min, ok) {
    ASSERT_EQ(min_of_3(1, 2, 3), 1);
    ASSERT_EQ(min_of_3(1, 3, 2), 1);
    ASSERT_EQ(min_of_3(2, 1, 3), 1);
    ASSERT_EQ(min_of_3(3, 1, 2), 1);
    ASSERT_EQ(min_of_3(2, 3, 1), 1);
    ASSERT_EQ(min_of_3(3, 2, 1), 1);
}
TEST(Levenstein, ok) {
    EXPECT_EQ(string_distance("Toyota", "Toyota"), 1);
    EXPECT_EQ(string_distance("Toyota", "Toy"), 0.5);
    EXPECT_EQ(string_distance("Toyota", "Renaul"), 0);
}
TEST(distance, ok) {
    EXPECT_EQ(distance_fl(1, 2), 0.5);
    EXPECT_EQ(distance_fl(4, 1), 0.25);
}


// tests for memory functions
TEST(allocate_string, ok) {
    char buffer[SIZE_BUF] = "test_string";
    char **allocated_string = nullptr;
    EXPECT_EQ(allocate_string(allocated_string, buffer), NULLPTR_EX);
    allocated_string = (char**) malloc(sizeof(char*));
    *allocated_string = nullptr;
    EXPECT_EQ(allocate_string(allocated_string, buffer), 0);
    free(*allocated_string);
    char buffer2[SIZE_BUF] = "";
    EXPECT_EQ(allocate_string(allocated_string, buffer2), INCORRECT_ENTRY);
    free(allocated_string);
}

TEST(open_car_database, ok) {
    FILE* pointer;
    EXPECT_EQ(open_car_database(&pointer, "../../db.txt"), 0);
    fclose(pointer);
    EXPECT_EQ(open_car_database(&pointer, "search.txt"), NULLPTR_EX);
}

TEST(free_car, ok) {
    car* pointer = NULL;
    car car_a = {100, 100, 1, NULL, NULL};
    char buffer[SIZE_BUF] = "test_string";
    allocate_string(&car_a.body_type, buffer);
    allocate_string(&car_a.model_name, buffer);
    EXPECT_EQ(free_car(pointer), NULLPTR_EX);
    EXPECT_EQ(free_car(&car_a), 0);
}



// as car_nullptr is only called on need of null allocation
// or on already nulled strings it has no memleak
TEST(car_nullptr, ok) {
    car car_a = {100, 100, 1, nullptr, nullptr};
    EXPECT_EQ(car_nullptr(&car_a), 0);
}

TEST(copy_car, not_ok) {
    car car_a = {100, 100, 1, (char*)"Toyota", (char*)"Sedan"};
    car* newcar = nullptr;
    EXPECT_EQ(copy_car(newcar, &car_a), NULLPTR_EX);
}

TEST(copy_car, ok) {
    car car_a = {100, 100, 1, (char*)"Toyota", (char*)"Sedan"};
    car newcar = {200, 0, 2, nullptr, nullptr};
    EXPECT_EQ(copy_car(&newcar, &car_a), 0);
    ASSERT_EQ(newcar.fuel_consumption, car_a.fuel_consumption);
    ASSERT_EQ(newcar.fuel_consumption, car_a.fuel_consumption);
    ASSERT_EQ(newcar.fuel_consumption, car_a.fuel_consumption);
    ASSERT_STREQ(newcar.body_type, car_a.body_type);
    ASSERT_STREQ(newcar.model_name, car_a.model_name);
    free_car(&newcar);
}

TEST(comparison, ok) {
    car car_a = {100, 100, 1, (char*)"Toyota", (char*)"Sedan"};
    car car_b = {100, 0, 1, (char*)"Toyota", (char*)"Sedan"};
    car car_c = {100, 0, 0, (char*)"Toyota", (char*)"Sedan"};
    car car_d = {0, 0, 0, (char*)"Renaul", (char*)"Coupe"};
    EXPECT_EQ(comparison(&car_a, &car_a), 5);
    EXPECT_EQ(comparison(&car_a, &car_b), 4);
    EXPECT_EQ(comparison(&car_a, &car_c), 3);
    EXPECT_EQ(comparison(&car_a, &car_d), 0);
}

TEST(print_car_instance, ok) {
    car car_a = {100, 100, 1, (char*)"Toyota", (char*)"Sedan"};
    ASSERT_EQ(print_car_instance(&car_a), 0);
}

TEST(read_car_instance, ok) {
    FILE* file = nullptr;
    car car_1 = {100, 100, 0, NULL, NULL};
    ASSERT_EQ(read_car_instance(file, &car_1), NULLPTR_EX);
    open_car_database(&file, "../../tests/5.txt");
    ASSERT_EQ(read_car_instance(file, &car_1), EOF_REACHED);
    fclose(file);
    open_car_database(&file, "../../db.txt");
    ASSERT_EQ(read_car_instance(file, &car_1), 0);
    fclose(file);
    free_car(&car_1);
}

TEST(read_car_instance, not_ok_1) {
    FILE* file = nullptr;
    car car_1 = {100, 100, 0, NULL, NULL};
    open_car_database(&file, "../../tests/1.txt");
    ASSERT_EQ(read_car_instance(file, &car_1), INCORRECT_ENTRY);
    free_car(&car_1);
    fclose(file);
    free_car(&car_1);
}

TEST(read_car_instance, not_ok_2) {
    FILE* file = nullptr;
    car car_1 = {100, 100, 0, NULL, NULL};
    open_car_database(&file, "../../tests/2.txt");
    read_car_instance(file, &car_1);
    ASSERT_EQ(read_car_instance(file, &car_1), INCORRECT_ENTRY);
    fclose(file);
    free_car(&car_1);
}

TEST(read_car_instance, not_ok_3) {
    FILE* file = nullptr;
    car car_1 = {100, 100, 0, NULL, NULL};
    open_car_database(&file, "../../tests/3.txt");
    read_car_instance(file, &car_1);
    ASSERT_EQ(read_car_instance(file, &car_1), INCORRECT_ENTRY);
    fclose(file);
    free_car(&car_1);
}

TEST(errprint, ok) {
    ASSERT_EQ(error_out(1), 1);
    ASSERT_EQ(error_out(2), 2);
    ASSERT_EQ(error_out(3), 3);
    ASSERT_EQ(error_out(4), 4);
    ASSERT_EQ(error_out(-1), 0);
}

TEST(search_test, ok) {
    car* input_car = (car*)malloc(sizeof(car));
    car* found_car = (car*)malloc(sizeof(car));
    car_nullptr(input_car);
    car_nullptr(found_car);
    FILE* input = fopen("../../tests/4.txt", "r");
    FILE* db = fopen("../../db.txt", "r");
    read_car_instance(input, input_car);
    search_in_base(input_car, found_car, db);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_STREQ(found_car->body_type, input_car->body_type);
    ASSERT_STREQ(found_car->model_name, input_car->model_name);
    free_car(input_car);
    fclose(db);
    db = fopen("../../db.txt", "r");
    read_car_instance(input, input_car);
    search_in_base(input_car, found_car, db);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_EQ(found_car->fuel_consumption, input_car->fuel_consumption);
    ASSERT_STREQ(found_car->body_type, input_car->body_type);
    ASSERT_STREQ(found_car->model_name, input_car->model_name);
    free_car(input_car);
    free_car(found_car);
    free(input_car);
    free(found_car);
    fclose(input);
    fclose(db);
}



