#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "node.hpp"

class Tree_t {
public:
    Node_t* root;      // Raw pointer to the root node
    std::string name;  // Name of the function/module
    int size = 0;      // Number of nodes in the tree

    explicit Tree_t(Node_t* r, std::string n)
        : root(r), name(std::move(n)) {
        if (root) size = GetSize(root);
    }

    // Since Node_t has a recursive destructor, we just delete the root
    ~Tree_t() {
        delete root;
    }

    // Prevent accidental copying (to avoid double free of root)
    Tree_t(const Tree_t&) = delete;
    Tree_t& operator=(const Tree_t&) = delete;

    // Recursive dump to console
    void PrintNode(Node_t* current = nullptr, int depth = 0) const;

    // Recursive size calculation
    size_t GetSize(Node_t* current = nullptr) const;
};