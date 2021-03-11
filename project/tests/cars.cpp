//
// Created by Борис Кожуро on 10.03.2021.
//

#include "gtest/gtest.h"

extern "C" {
#include "cars.h"
}

TEST(min, ok) {
    ASSERT_EQ(min_of_3(1,2,3), 1);
    ASSERT_EQ(min_of_3(1,3,2), 1);
    ASSERT_EQ(min_of_3(2,1,3), 1);
    ASSERT_EQ(min_of_3(3,1,2), 1);
    ASSERT_EQ(min_of_3(2,3,1), 1);
    ASSERT_EQ(min_of_3(3,2,1), 1);
}




