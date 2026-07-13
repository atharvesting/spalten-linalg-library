cmake_minimum_required(VERSION 3.14)
project(Spalten LANGUAGES CXX)

add_library(Spalten INTERFACE)

# Expose the include directory to users so they can write #include <Spalten/Matrix.hpp>
target_include_directories(Spalten INTERFACE ${CMAKE_SOURCE_DIR}/include)