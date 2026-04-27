#include <benchmark/benchmark.h>
#include "gemm.hpp"

static void BM_GemmNaive(benchmark::State& state) {
    size_t dim = state.range(0);
    kernels::Tensor A(1, dim, dim, 1), B(1, dim, dim, 1), C(1, dim, dim, 1);
    for (auto _ : state) {
        kernels::gemm_naive(A, B, C);
    }
}
BENCHMARK(BM_GemmNaive)->RangeMultiplier(2)->Range(64, 512);

static void BM_GemmAVX(benchmark::State& state) {
    size_t dim = state.range(0);
    kernels::Tensor A(1, dim, dim, 1), B(1, dim, dim, 1), C(1, dim, dim, 1);
    for (auto _ : state) {
        kernels::gemm_avx(A, B, C);
    }
}
BENCHMARK(BM_GemmAVX)->RangeMultiplier(2)->Range(64, 1024);
BENCHMARK(BM_Conv_Naive)->RangeMultiplier(2)->Range(64, 512);
BENCHMARK(BM_Conv_Im2Col_AVX)->RangeMultiplier(2)->Range(64, 512);

BENCHMARK_MAIN();