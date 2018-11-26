cmake_minimum_required(VERSION 3.5)
project(List LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(core-files
        autocorrect.cpp
        autocorrect.hpp
    tests-main.cpp
    test-helpers.hpp
    test-helpers.cpp
    catch.hpp
)

add_executable(tests
        tests-autocorrect.cpp
    ${core-files}
)

set(binaries
        tests
)

foreach(target ${BINARIES})
    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
        target_compile_options( ${target} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
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
