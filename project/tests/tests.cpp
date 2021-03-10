//
// Created by Борис Кожуро on 10.03.2021.
//

#include "gtest/gtest.h"

extern "C" {
#include "cars.h"
}

TEST(min, ok) {
ASSERT_EQ(min_of_3(1,2,3), 1);
}



