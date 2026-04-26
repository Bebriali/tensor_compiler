cmake_minimum_required(VERSION 3.14)
project(KernelsOptimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    # Заменяем avx2 на sse4.2
    add_compile_options(-O3 -msse4.2 -mfma)
endif()
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)
FetchContent_MakeAvailable(googlebenchmark)

add_library(kernels_lib
    src/gemm.cpp
    src/convolution.cpp
)
target_include_directories(kernels_lib PUBLIC include)

add_executable(unit_tests test/test_kernels.cpp)
target_link_libraries(unit_tests PRIVATE kernels_lib GTest::gtest_main)

add_executable(bench_kernels bench/bench_kernels.cpp)
target_link_libraries(bench_kernels PRIVATE kernels_lib benchmark::benchmark)
