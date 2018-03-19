cmake_minimum_required(VERSION 3.5.1)
project(02_Threadpool)

set(CMAKE_CXX_STANDARD 14)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -O3" )

add_executable(threadpool.bin main.cpp ThreadPool.cpp)
add_custom_target(threadpool COMMAND threadpool.bin DEPENDS threadpool.bin)
