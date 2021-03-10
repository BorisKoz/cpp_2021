cmake_minimum_required(VERSION 3.17)
project(project C)

set(CMAKE_C_STANDARD 99)

file(GLOB sources
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(main ${sources})
