cmake_minimum_required(VERSION 3.20)
project(clifx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
