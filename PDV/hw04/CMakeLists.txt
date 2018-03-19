cmake_minimum_required(VERSION 3.5.1)
project(PDV_HW03)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")

add_executable(PDV_HW03 main.cpp tests.h bst_tree.cpp bst_tree.h)