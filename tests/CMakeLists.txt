include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(clifx_tests
    test_packable.cpp
    test_base.cpp
    test_getters.cpp
    test_setters.cpp
)

target_link_libraries(clifx_tests PRIVATE clifx GTest::gtest_main)

target_include_directories(clifx_tests PRIVATE ${CMAKE_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(clifx_tests)
