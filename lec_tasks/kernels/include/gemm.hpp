#pragma once
#include "tensor.hpp"

namespace kernels {
    // Матрица A (M x K), B (K x N) -> C (M x N)
    void gemm_naive(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_cache_friendly(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_tiled(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_avx(const Tensor& A, const Tensor& B, Tensor& C);
}
