cmake_minimum_required(VERSION 3.5)
project(flatset LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tests-basic
    flatset.hpp
    tests-flatset-basic.cpp
    tests-main.cpp
    catch.hpp
    tests-helpers.cpp
    tests-helpers.hpp
)

add_executable(tests-full
    flatset.hpp
    tests-flatset-full.cpp
    tests-flatset-complexity.cpp
    tests-main.cpp
    catch.hpp
    tests-helpers.cpp
    tests-helpers.hpp
)


if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( tests-basic PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic )
    target_compile_options( tests-full PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options( tests-basic PRIVATE /W4 )
    target_compile_options( tests-full PRIVATE /W4 )
endif()


if (${CMAKE_VERSION} VERSION_GREATER 3.5)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tests-basic)
endif()


include(CTest)
add_test(NAME basics COMMAND tests-basic)
add_test(NAME full COMMAND tests-full ~[long])

if ( CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo" )
    message(STATUS "Registering the complexities test to CTest")
    add_test(NAME complexities COMMAND tests-full [.long])
endif()
