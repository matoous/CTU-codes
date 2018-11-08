cmake_minimum_required(VERSION 3.5.1)
project(ParallelSortingHW)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -std=c++14 -O3")

add_executable(hw main.cpp sort.cpp)