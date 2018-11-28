cmake_minimum_required(VERSION 3.5)
project(symcalc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_SOURCES
        tokenizer.cpp
        tokenizer.hpp
        cmdline.cpp
        cmdline.hpp
        expr.hpp
        expr.cpp
        expr_impl.cpp
        expr_impl.hpp
        app.hpp
        app.cpp
        # Případné další soubory přidávejte sem
        # Funkce main musí být v souboru main.cpp, který se linkuje jen pro target symcalc
        )

# Soubory pro tests-direct
set(TEST_SOURCES
        tests/tests-main.cpp
        tests/tests-tokenizer.cpp
        tests/tests-cmdline.cpp
        tests/tests-expr.cpp
        tests/tests-utils.cpp
        )

# Soubory pro tests-runner-*
set(TEST_RUNNER_SOURCES
        tests/tests-main.cpp
        tests/tests-expr.cpp
        tests/tests-expr-long.cpp
        tests/tests-generated.cpp
        tests/tests-utils.cpp
        )

# Implementace spuštění subprocesu pro danou platformu. Potřeba jen pro tests-runner-subprocess.
if ( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set(SUBPROCESS_IMPL tests/subprocess_windows.cpp)
elseif ( CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin" )
    set(SUBPROCESS_IMPL tests/subprocess_posix.cpp)
else()
    error("Unsupported system!")
endif()

# Vlastní aplikace.
add_executable(symcalc ${COMMON_SOURCES} main.cpp)

# Catch unit testy, které přímo volají implementované funkce.
add_executable(tests-direct ${COMMON_SOURCES} ${TEST_SOURCES})

# Testy, které přímo nekompilují váš kód, ale spouští ho jako samostatný program.
# Toto je použito v odevzdávacím systému pro hodnocení.
add_executable(tests-runner-subprocess ${TEST_RUNNER_SOURCES} tests/runner_subprocess.cpp ${SUBPROCESS_IMPL})

# Obdoba předchozích testů, s tím rozdílem, že se vás program nespouští jako
# samostatný proces, ale volá se funkce handle_expr_line. Tím pádem je jednodušší
# program při testech ladit a např. pustit pod valgrindem.
add_executable(tests-runner-direct ${COMMON_SOURCES} ${TEST_RUNNER_SOURCES} tests/runner_direct.cpp)


target_compile_definitions(tests-runner-subprocess PRIVATE -DRUNNER)
target_compile_definitions(tests-runner-direct PRIVATE -DRUNNER)

# Chceme být schopní includovat knihovnu mapbox pomocí <>
include_directories(.)

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    add_compile_options(-Wall -Wextra -Wunreachable-code -Wpedantic)
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    add_compile_options(/W4)
endif()


if (${CMAKE_VERSION} VERSION_GREATER 3.5)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tests-direct)
endif()

# zajistí, že symcalc se vždy zkompiluje před testerem a ten bude testovat aktuální kód
add_dependencies(tests-runner-subprocess symcalc)

include(CTest)
add_test(NAME direct COMMAND tests-direct)
add_test(NAME runner-direct COMMAND tests-runner-direct)
add_test(NAME runner-subprocess COMMAND tests-runner-subprocess)

# test-runner-subprocess očekává v proměnné prostředí TEST_CMD příkaz pro spuštění
# programu symcalc. To může být např. i "valgrind ./symcalc"
if ( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set(TEST_CMD "symcalc")
else()
    set(TEST_CMD "./symcalc")
endif()

set_tests_properties(runner-subprocess PROPERTIES ENVIRONMENT TEST_CMD="${TEST_CMD}")

if ( CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo" )
    message(STATUS "Registering long tests to CTest")
    add_test(NAME complex-tests-direct COMMAND tests-runner-direct [.long])
    add_test(NAME complex-tests-subprocess COMMAND tests-runner-subprocess [.long])
    set_tests_properties(complex-tests-direct PROPERTIES ENVIRONMENT TEST_CMD="${TEST_CMD}")
    set_tests_properties(complex-tests-subprocess PROPERTIES ENVIRONMENT TEST_CMD="${TEST_CMD}")
endif()
