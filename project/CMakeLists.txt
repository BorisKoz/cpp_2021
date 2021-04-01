cmake_minimum_required(VERSION 3.15)
project(files)

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")

enable_testing()


include_directories("${PROJECT_SOURCE_DIR}/include")


set(INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(SOURCE ${PROJECT_SOURCE_DIR}/src)

add_library(bigfile_linear STATIC
        ${INCLUDE}/search.h
        ${SOURCE}/linear.c)
add_library(bigfile_parallel SHARED
        ${INCLUDE}/search.h
        ${SOURCE}/parallel.c)

file(GLOB prod_sources
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/main.c")

add_executable(main.out ${PROJECT_SOURCE_DIR}/src/main.c)
target_link_libraries(main.out -ldl bigfile_linear)


file(GLOB tests "${PROJECT_SOURCE_DIR}/tests/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/tests/main.cpp")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${PROJECT_SOURCE_DIR}/src/${name}.c
            ${file}
            "${PROJECT_SOURCE_DIR}/tests/main.cpp")
    target_link_libraries("${name}_tests" gtest_main)
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()