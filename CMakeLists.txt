cmake_minimum_required(VERSION 3.20)
project(clifx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CLIFX_ENABLE_COVERAGE "Enable coverage instrumentation (gcov/lcov)" OFF)
option(CLIFX_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(CLIFX_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

add_library(clifx_project_options INTERFACE)

if(CLIFX_ENABLE_COVERAGE)
    target_compile_options(clifx_project_options INTERFACE --coverage -O0 -g)
    target_link_options(clifx_project_options INTERFACE --coverage)
endif()

if(CLIFX_ENABLE_ASAN)
    target_compile_options(clifx_project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(clifx_project_options INTERFACE -fsanitize=address)
endif()

if(CLIFX_ENABLE_UBSAN)
    target_compile_options(clifx_project_options INTERFACE -fsanitize=undefined)
    target_link_options(clifx_project_options INTERFACE -fsanitize=undefined)
endif()

add_subdirectory(utils)
add_subdirectory(packets)

add_library(clifx INTERFACE)

target_link_libraries(clifx INTERFACE packets)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(CLIFX_BUILD_TESTS "Build clifx unit tests" ON)
else()
    option(CLIFX_BUILD_TESTS "Build clifx unit tests" OFF)
endif()

if(CLIFX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
