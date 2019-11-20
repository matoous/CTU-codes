cmake_minimum_required(VERSION 3.5)
project(List LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(core-files
    trie.cpp
    trie.hpp
    tests-main.cpp
    catch.hpp
)

set(helper-files
    test-helpers.hpp
    test-helpers.cpp
)

add_executable(tests-stage-1
    tests-trie-01.cpp
    ${core-files}
)

add_executable(tests-stage-2
    tests-trie-02.cpp
    ${core-files}
    ${helper-files}
)

add_executable(tests-stage-3
    tests-trie-03.cpp
    ${core-files}
    ${helper-files}
)

add_executable(tests-stage-4
    tests-trie-04.cpp
    ${core-files}
    ${helper-files}
)

add_executable(tests-stage-5
    tests-trie-05.cpp
    ${core-files}
    ${helper-files}
)

set(binaries
    tests-stage-1
    tests-stage-2
    tests-stage-3
    tests-stage-4
    tests-stage-5
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
add_test(NAME stage-1 COMMAND tests-stage-1)
add_test(NAME stage-2 COMMAND tests-stage-2)
add_test(NAME stage-3 COMMAND tests-stage-3)
add_test(NAME stage-4 COMMAND tests-stage-4)
add_test(NAME stage-5 COMMAND tests-stage-5)

if ( CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo" )
    message(STATUS "Registering the complexities test to CTest")
    add_test(NAME complexities COMMAND tests-stage-5 [.long])
endif()
