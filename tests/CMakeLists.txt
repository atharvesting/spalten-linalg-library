cmake_minimum_required(VERSION 3.14)
project(Spalten_Test LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Spalten_Test
    pch.cpp
    pch.h
    test_vector.cpp
    test_matrix.cpp
)

target_link_libraries(Spalten_Test PRIVATE
    Spalten
    gtest_main
)

add_test(NAME RunTests COMMAND Spalten_Test)