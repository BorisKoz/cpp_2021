// Copyright 2021 <BKZ>
#include "gtest/gtest.h"

extern "C" {
#include "../include/search.h"
}

TEST(file_search_parallel, null_params) {
    remove("1.txt");
    FILE* f1 = fopen("1.txt", "a");
    char* c1 = (char*)calloc(10, sizeof (char));
    size_t* found = (size_t*)calloc(10, sizeof (size_t));
    size_t stf = 0;
    ASSERT_EQ(file_search(nullptr, c1, found, stf), NULL_ENTRY);
    ASSERT_EQ(file_search(&f1, nullptr, found, stf), NULL_ENTRY);
    ASSERT_EQ(file_search(&f1, c1, nullptr, stf), NULL_ENTRY);
    free(c1);
    free(found);
}


TEST(file_search_parallel, empty_file) {
    remove("1.txt");
    FILE* f1 = fopen("1.txt", "a");
    char* c1 = (char*)calloc(10, sizeof (char));
    size_t* found = (size_t*)calloc(10, sizeof (size_t));
    size_t stf = 0;
    ASSERT_EQ(file_search(&f1, c1, found, stf), NULL_ENTRY);
    free(c1);
    free(found);
}

TEST(file_search_parallel, premade_file) {
    remove("1.txt");
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
    EXPECT_EQ(found[0], 4);
    EXPECT_EQ(found[1], 2);
    EXPECT_EQ(found[2], 3);
}

TEST(file_search_parallel, grep_random) {
    remove("1.txt");
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
    remove("1.txt");
    f1 = fopen("grep_results.txt", "r");
    int i1, i2;
    fscanf(f1, "%d %d", &i1, &i2);
    fclose(f1);
    EXPECT_EQ(found[0], i1);
    EXPECT_EQ(found[1], i2);
}
