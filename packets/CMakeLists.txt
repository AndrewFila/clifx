add_library(packets STATIC
    base.cpp
    getters.cpp
    setters.cpp
)

target_include_directories(packets PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(packets PUBLIC utils)

target_compile_options(packets PRIVATE
    -Wall -Wextra -Wpedantic
)
