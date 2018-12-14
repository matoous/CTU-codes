cmake_minimum_required(VERSION 3.5)
project(List LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)

SET(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(core-files
        autocorrect.cpp
        autocorrect.hpp
        tests-main.cpp
        test-helpers.hpp
        test-helpers.cpp
        catch.hpp)

set(cli-files
        autocorrect.cpp
        autocorrect.hpp)

set(benchmark-files
        autocorrect.cpp
        autocorrect.hpp
        test-helpers.hpp
        test-helpers.cpp
        picobench.hpp)

add_executable(tests
        tests-autocorrect.cpp
    ${core-files}
)

add_executable(benchmark
        benchmark-autocorrect.cpp
        ${benchmark-files}
        )

add_executable(cli
        cli.cpp
        ${cli-files}
        )

find_package(Threads REQUIRED)
target_link_libraries(tests Threads::Threads)
target_link_libraries(benchmark Threads::Threads)
target_link_libraries(cli Threads::Threads)

set(binaries
        tests
        cli
        benchmark
)

foreach(target ${BINARIES})
    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
        target_compile_options( ${target} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -lpthread)
    endif()
    if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
        target_compile_options( ${target} PRIVATE /W4 )
        target_compile_options( ${target} PRIVATE /utf-8 )
    endif()
endforeach()

enable_testing()
add_test(NAME tests COMMAND tests)

if ( CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo" )
    message(STATUS "Registering the complexities test to CTest")
    add_test(NAME complexities COMMAND tests-stage-5 [.long])
endif()
