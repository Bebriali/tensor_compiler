#pragma once

#include <deque>
#include <memory>

#include "node.hpp"

class Stack_t
{
    public:
        size_t capacity;
        size_t cur_size;
        std::deque<std::shared_ptr<Node_t>> nodes;

        explicit Stack_t(size_t cap)
            : capacity(cap), cur_size(0) {}

        void PushBack(std::shared_ptr<Node_t> node) {
            if (cur_size < capacity) {
                nodes.push_back(node);
                cur_size++;
            }
        }

        std::shared_ptr<Node_t> PopBack() {
            if (!nodes.empty()) {
                auto node = nodes.back();
                nodes.pop_back();
                cur_size--;
                return node;
            }
            return nullptr;
        }

        void PushFront(std::shared_ptr<Node_t> node) {
            if (cur_size < capacity) {
                nodes.push_front(node);
                cur_size++;
            }
        }

        std::shared_ptr<Node_t> PopFront() {
            if (!nodes.empty()) {
                auto node = nodes.front();
                nodes.pop_front();
                cur_size--;
                return node;
            }
            return nullptr;
        }

        bool IsEmpty() const { return nodes.empty(); }
};
