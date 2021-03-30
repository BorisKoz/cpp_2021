// Copyright 2021 <BKZ>
#include "gtest/gtest.h"

extern "C" {
#include "../include/search.h"
}

TEST(file_search, null_params) {
    FILE* f1 = fopen("1.txt", "a");
    char* c1 = (char*)calloc(10, sizeof (char));
    size_t* found = (size_t*)calloc(10, sizeof (size_t));
    size_t stf = 0;
    ASSERT_EQ(file_search(nullptr, c1, found, stf), NULL_ENTRY);
    ASSERT_EQ(file_search(&f1, nullptr, found, stf), NULL_ENTRY);
    ASSERT_EQ(file_search(&f1, c1, nullptr, stf), NULL_ENTRY);
}


TEST(file_search, empty_file) {
    FILE* f1 = fopen("1.txt", "a");
    char* c1 = (char*)calloc(10, sizeof (char));
    size_t* found = (size_t*)calloc(10, sizeof (size_t));
    size_t stf = 0;
    ASSERT_EQ(file_search(&f1, c1, found, stf), NULL_ENTRY);
}

TEST(file_search, premade_file) {
    FILE* f1 = fopen("1.txt", "a");
    fputs("a a a a\n b\n b\n", f1);
    char c1[3] = {'a', 'b', '\n'};
    size_t found[3] = {0, 0, 0};
    size_t stf = 3;
    fclose(f1);
    f1 = fopen("1.txt", "r");
    file_search(&f1, c1, found, stf);
    fclose(f1);
    remove("1.txt");
    ASSERT_EQ(found[0], 4);
    ASSERT_EQ(found[1], 2);
    ASSERT_EQ(found[2], 3);
}

TEST(file_search, grep_random) {
    FILE* f1 = fopen("1.txt", "a");
    char c1[3] = {'a', 'b', '\n'};
    size_t found[3] = {0, 0, 0};
    size_t stf = 3;
    fclose(f1);
    system("LC_CTYPE=C tr -dc A-Za-z0-9 < /dev/urandom | "
           "fold -w 100 | head -n 10000 > 1.txt ");
    f1 = fopen("1.txt", "r");
    file_search(&f1, c1, found, stf);
    fclose(f1);
    remove("grep_results.txt");
    system("grep -o 'a' 1.txt | wc -l >> grep_results.txt");
    system("grep -o 'b' 1.txt | wc -l >> grep_results.txt");
    system("grep -o '\n' 1.txt | wc -l >> grep_results.txt");
    remove("1.txt");
    f1 = fopen("grep_results.txt", "r");
    int i1, i2, i3;
    fscanf(f1, "%d %d %d", &i1, &i2, &i3);
    fclose(f1);
    ASSERT_EQ(found[0], i1);
    ASSERT_EQ(found[1], i2);
    ASSERT_EQ(found[2], i3);
}
