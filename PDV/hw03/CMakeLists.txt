cmake_minimum_required(VERSION 3.5.1)
project(hw)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fopenmp -Wunused-parameter")

add_executable(hw.bin main.cpp _dataGenerator/DataGenerator.cpp SumsOfVectors.cpp SumsOfVectors.h _executor/Executor.cpp _executor/Executor.h _outputGenerator/TextTable.h)

add_custom_target(hw COMMAND hw.bin DEPENDS hw.bin)