cmake_minimum_required(VERSION 3.20)
project(clifx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(utils)
add_subdirectory(packets)

add_library(clifx INTERFACE)

target_link_libraries(clifx INTERFACE packets)
