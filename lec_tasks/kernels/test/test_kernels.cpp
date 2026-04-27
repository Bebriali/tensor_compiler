#include <gtest/gtest.h>
#include "gemm.hpp"
#include "convolution.hpp"
#include <cmath>

void ExpectTensorEq(const kernels::Tensor& a, const kernels::Tensor& b) {
    ASSERT_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        EXPECT_NEAR(a.data_ptr()[i], b.data_ptr()[i], 1e-4);
    }
}

TEST(GemmTest, AvxMatchesNaive) {
    size_t M = 16, K = 16, N = 16;
    kernels::Tensor A(1, M, K, 1), B(1, K, N, 1), C_naive(1, M, N, 1), C_avx(1, M, N, 1);
    
    for (size_t i = 0; i < A.size(); ++i) A.data_ptr()[i] = static_cast<float>(i % 10);
    for (size_t i = 0; i < B.size(); ++i) B.data_ptr()[i] = static_cast<float>(i % 7);

    kernels::gemm_naive(A, B, C_naive);
    kernels::gemm_avx(A, B, C_avx);

    ExpectTensorEq(C_naive, C_avx);
}

TEST(ConvTest, Im2ColCorrectness) {
    kernels::Tensor input(1, 5, 5, 1); // 5x5 image
    kernels::Tensor kernel(1, 3, 3, 1); // 3x3 kernel
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}