cmake_minimum_required(VERSION 3.14)
project(Spalten LANGUAGES CXX)

add_library(Spalten INTERFACE)

target_include_directories(Spalten INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})