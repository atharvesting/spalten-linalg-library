cmake_minimum_required(VERSION 3.14)
project(SpaltenSolution VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the header-only library target
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)

    # Separate manual-test executable using tests/main.cpp
    add_executable(SpaltenManualTest tests/main.cpp)
    target_link_libraries(SpaltenManualTest PRIVATE Spalten)
endif()

set_target_properties(Spalten PROPERTIES LINKER_LANGUAGE CXX)

# Temporary debug print inside your subdirectory:
message(STATUS "Current CXX Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Debug CXX Flags: ${CMAKE_CXX_FLAGS_DEBUG}")