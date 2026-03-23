#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "node.hpp"

class Tree_t
{
public:
    std::shared_ptr<Node_t> root;
    std::string name;

    int size = 0;

    explicit Tree_t(std::shared_ptr<Node_t> r, std::string n)
        : root(std::move(r)), name(std::move(n)) {}

    void PrintNode(std::shared_ptr<Node_t> current = nullptr, int depth = 0) const;
    size_t GetSize(std::shared_ptr<Node_t> current = nullptr) const;
};
