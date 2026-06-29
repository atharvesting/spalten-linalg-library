cmake_minimum_required(VERSION 3.14)
project(Spalten LANGUAGES CXX)

add_library(Spalten STATIC
    matrix.h
    utils.h
)

target_include_directories(Spalten PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})