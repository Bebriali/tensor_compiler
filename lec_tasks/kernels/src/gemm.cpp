#include "gemm.hpp"
#include <immintrin.h>
#include <algorithm>

namespace kernels {

void gemm_naive(const Tensor& A, const Tensor& B, Tensor& C) {
    size_t M = A.shape().H, K = A.shape().W, N = B.shape().W;
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            for (size_t k = 0; k < K; ++k)
                C(0, i, j, 0) += A(0, i, k, 0) * B(0, k, j, 0);
}

void gemm_cache_friendly(const Tensor& A, const Tensor& B, Tensor& C) {
    size_t M = A.shape().H, K = A.shape().W, N = B.shape().W;
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            float a_val = A(0, i, k, 0);
            for (size_t j = 0; j < N; ++j) {
                C(0, i, j, 0) += a_val * B(0, k, j, 0);
            }
        }
    }
}

void gemm_avx(const Tensor& A, const Tensor& B, Tensor& C) {
    size_t M = A.shape().H, K = A.shape().W, N = B.shape().W;
    const float* a_ptr = A.data_ptr();
    const float* b_ptr = B.data_ptr();
    float* c_ptr = C.data_ptr();

    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            __m256 a_vec = _mm256_set1_ps(a_ptr[i * K + k]);
            
            size_t j = 0;
            size_t vec_limit = (N / 8) * 8;
            
            for (; j < vec_limit; j += 8) {
                __m256 b_vec = _mm256_loadu_ps(&b_ptr[k * N + j]);
                __m256 c_vec = _mm256_loadu_ps(&c_ptr[i * N + j]);
                
                // FMA: c = a * b + c
                c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                
                _mm256_storeu_ps(&c_ptr[i * N + j], c_vec);
            }

            // tales handling
            float a_val = a_ptr[i * K + k];
            for (; j < N; ++j) {
                c_ptr[i * N + j] += a_val * b_ptr[k * N + j];
            }
        }
    }
}

void gemm_avx_ptr(const float* a_ptr, const float* b_ptr, float* c_ptr, size_t M, size_t K, size_t N) {
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            __m256 a_vec = _mm256_set1_ps(a_ptr[i * K + k]);
            
            size_t j = 0;
            size_t vec_limit = (N / 8) * 8;
            
            for (; j < vec_limit; j += 8) {
                __m256 b_vec = _mm256_loadu_ps(&b_ptr[k * N + j]);
                __m256 c_vec = _mm256_loadu_ps(&c_ptr[i * N + j]);
                
                c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                
                _mm256_storeu_ps(&c_ptr[i * N + j], c_vec);
            }

            // Хвосты
            float a_val = a_ptr[i * K + k];
            for (; j < N; ++j) {
                c_ptr[i * N + j] += a_val * b_ptr[k * N + j];
            }
        }
    }
}
}