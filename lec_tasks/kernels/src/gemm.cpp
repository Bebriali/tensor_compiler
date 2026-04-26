#include <immintrin.h>

void gemm_sse(const Tensor& A, const Tensor& B, Tensor& C) {
    size_t M = A.shape().H, K = A.shape().W, N = B.shape().W;
    const float* a_ptr = A.data_ptr();
    const float* b_ptr = B.data_ptr();
    float* c_ptr = C.data_ptr();

    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            // Загружаем одно значение A и дублируем его в 4 позиции (128 бит)
            __m128 a_vec = _mm_set1_ps(a_ptr[i * K + k]);

            for (size_t j = 0; j < (N / 4) * 4; j += 4) {
                __m128 b_vec = _mm_loadu_ps(&b_ptr[k * N + j]);
                __m128 c_vec = _mm_loadu_ps(&c_ptr[i * N + j]);

                // Используем FMA (если есть) или просто mul + add
                c_vec = _mm_fmadd_ps(a_vec, b_vec, c_vec);

                _mm_storeu_ps(&c_ptr[i * N + j], c_vec);
            }
            // Здесь добавь цикл для обработки остатка (N % 4)
        }
    }
}
