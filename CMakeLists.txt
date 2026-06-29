cmake_minimum_required(VERSION 3.14)
project(SpaltenSolution VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Spalten)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(Spalten_Test)
endif()

set_target_properties(Spalten PROPERTIES LINKER_LANGUAGE CXX)