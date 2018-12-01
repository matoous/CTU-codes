cmake_minimum_required(VERSION 3.5.1)
project(DatabaseQueries)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(DatabaseQueries main.cpp query.h _generator/generator.cpp)