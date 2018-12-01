cmake_minimum_required(VERSION 3.5.1)
project(code)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -Wall")

add_executable(main.bin main.cpp integrate.cpp integrate.h)
add_executable(test_openmp.bin test.cpp)

add_custom_target(main COMMAND main.bin DEPENDS main.bin)
add_custom_target(test_openmp COMMAND test_openmp.bin DEPENDS test_openmp.bin)
