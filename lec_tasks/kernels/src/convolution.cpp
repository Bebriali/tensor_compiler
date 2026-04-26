#include "tensor.hpp"
#include "gemm.hpp"
#include <vector>

namespace kernels {

void im2col(const Tensor& input, int K, int stride, float* data_col) {
    int C = input.shape().C;
    int H = input.shape().H;
    int W = input.shape().W;
    int OH = (H - K) / stride + 1;
    int OW = (W - K) / stride + 1;

    for (int c = 0; c < C; ++c) {
        for (int ky = 0; ky < K; ++ky) {
            for (int kx = 0; kx < K; ++kx) {
                for (int y = 0; y < OH; ++y) {
                    for (int x = 0; x < OW; ++x) {
                        data_col[((c * K * K + ky * K + kx) * OH * OW) + y * OW + x] =
                            input(0, y * stride + ky, x * stride + kx, c);
                    }
                }
            }
        }
    }
}

Tensor convolve_im2col(const Tensor& input, const Tensor& kernel, int stride) {
    int M = kernel.shape().N; // Число фильтров
    int K = kernel.shape().H;
    int C = input.shape().C;
    int OH = (input.shape().H - K) / stride + 1;
    int OW = (input.shape().W - K) / stride + 1;

    // 1. Создаем буфер для развернутой матрицы
    std::vector<float> col_buffer(C * K * K * OH * OW);
    im2col(input, K, stride, col_buffer.data());

    // 2. Свертка превратилась в умножение матриц:
    // Weights (M x CK^2) * ColBuffer (CK^2 x OH*OW) = Output (M x OH*OW)
    Tensor output(1, OH, OW, M);
    // Здесь вызываем наш gemm_avx, адаптировав указатели
    // ... логика вызова GEMM ...

    return output;
}
}
