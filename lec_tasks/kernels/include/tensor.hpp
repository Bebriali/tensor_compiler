#pragma once
#include <vector>
#include <iostream>
#include <memory>

namespace kernels {
    struct Shape {
        size_t N, H, W, C;
    };

    class Tensor {
    public:
        Tensor(size_t n, size_t h, size_t w, size_t c)
            : shape_{n, h, w, c}, data(n * h * w * c, 0.0f) {}

        float& operator()(size_t n, size_t h, size_t w, size_t c) {
            return data[((n * shape_.H + h) * shape_.W + w) * shape_.C + c];
        }

        const float& operator()(size_t n, size_t h, size_t w, size_t c) const {
            return data[((n * shape_.H + h) * shape_.W + w) * shape_.C + c];
        }

        float* data_ptr() { return data.data(); }
        const float* data_ptr() const { return data.data(); }
        const Shape& shape() const { return shape_; }
        size_t size() const { return data.size(); }

    private:
        Shape shape_;
        std::vector<float> data; // В продакшене лучше использовать aligned_alloc
    };
}
