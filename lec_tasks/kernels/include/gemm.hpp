#pragma once
#include "tensor.hpp"

namespace kernels {
    // matrix A (M x K), B (K x N) -> C (M x N)
    void gemm_naive(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_cache_friendly(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_tiled(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_avx(const Tensor& A, const Tensor& B, Tensor& C);
    void gemm_avx_ptr(const float* a_ptr, const float* b_ptr, float* c_ptr, size_t M, size_t K, size_t N);
}