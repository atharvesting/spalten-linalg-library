cmake_minimum_required(VERSION 3.14)
project(Spalten LANGUAGES CXX)

add_library(Spalten INTERFACE)

# Expose include directory cleanly for build and install interfaces
target_include_directories(Spalten INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Enforce modern C++ standard required for alignment and SIMD constructs
target_compile_features(Spalten INTERFACE cxx_std_23)

# Compiler-specific optimization and vectorization flags
if(MSVC)
    target_compile_options(Spalten INTERFACE
        /O2         # Maximum speed
        /Ob3        # Aggressive function inlining
        /fp:fast    # Relaxes FP model to allow vectorization across reductions
        /arch:AVX2  # Targets AVX2 SIMD instructions (change to /arch:AVX512 if available)
    )
else() # GCC, Clang, AppleClang
    target_compile_options(Spalten INTERFACE
        -O3             # Aggressive optimization and auto-vectorization passes
        -march=native   # Emits SIMD instructions (AVX2, AVX-512, NEON) available on host CPU
        -ffast-math     # Allows FP reassociation needed for SIMD reduction loops
    )
endif()