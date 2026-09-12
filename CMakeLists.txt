cmake_minimum_required(VERSION 3.20)
project(clifx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect sources
set(SOURCES
    app/clifx.cpp
    app/cliApp.cpp
    packets/base.cpp
    packets/getters.cpp
    packets/setters.cpp
)

add_executable(clifx ${SOURCES})

target_include_directories(clifx PRIVATE
    app
    packets
    utils
)

target_compile_options(clifx PRIVATE
    -Wall -Wextra -Wpedantic
)

# Test executable
set(TEST_SOURCES
    app/test.cpp
    packets/base.cpp
    packets/getters.cpp
    packets/setters.cpp
)

add_executable(clifx_test ${TEST_SOURCES})

target_include_directories(clifx_test PRIVATE
    app
    packets
    utils
)

target_compile_options(clifx_test PRIVATE
    -Wall -Wextra -Wpedantic
)
