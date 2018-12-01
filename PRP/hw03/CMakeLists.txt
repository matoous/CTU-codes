cmake_minimum_required(VERSION 3.6)
project(hw03)

set(CMAKE_C_FLAGS "-pedantic -Wall -Werror -std=c99 -O3")

set(SOURCE_FILES main.c)
add_executable(program ${SOURCE_FILES})