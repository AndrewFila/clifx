cmake_minimum_required(VERSION 3.20)
project(clifx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    packets/base.cpp
    packets/getters.cpp
    packets/setters.cpp
)

add_library(clifx STATIC ${SOURCES})

target_include_directories(clifx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(clifx PRIVATE
    -Wall -Wextra -Wpedantic
)
