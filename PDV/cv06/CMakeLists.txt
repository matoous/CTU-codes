cmake_minimum_required(VERSION 3.5.1)
project(sorting)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -O2")

add_executable(sorting sorting.cpp)
add_executable(prefixsum _prefixsum/prefixsum.cpp)