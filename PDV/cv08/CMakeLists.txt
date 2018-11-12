cmake_minimum_required(VERSION 3.5.1)
project(mult)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(mult main.cpp  sparse.h multiply.cpp multiply.h)