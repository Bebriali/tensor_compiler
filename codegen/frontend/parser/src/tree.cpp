#include "Tree_t.hpp"

size_t Tree_t::GetSize(Node_t* current) const {
    if (!current) return 0;
    size_t count = 1;
    for (Node_t* child : current->children) {
        count += GetSize(child);
    }
    return count;
}

void Tree_t::PrintNode(Node_t* current, int depth) const {
    // If no node provided, start from root
    if (current == nullptr && depth == 0) {
        current = root;
        if (!current) return;
        std::cout << "Tree: " << name << " (Size: " << size << ")" << std::endl;
    }

    if (!current) return;

    // Create indentation
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    // Print current node info
    std::cout << "|-- ";

    // Using your Node_t::Dump logic for the node content
    switch (current->type) {
        case TokenType::ID:     std::cout << "[ID] " << std::get<std::string>(current->data); break;
        case TokenType::OP:     std::cout << "[OP] " << std::get<std::string>(current->data); break;
        case TokenType::NUM:
            if (std::holds_alternative<int>(current->data)) std::cout << "[NUM] " << std::get<int>(current->data);
            else std::cout << "[NUM] " << std::get<double>(current->data);
            break;
        case TokenType::BLOCK:  std::cout << "[BLOCK] " << std::get<std::string>(current->data); break;
        default:                std::cout << "[TOKEN] " << (int)current->type; break;
    }
    std::cout << std::endl;

    // Recurse for children
    for (Node_t* child : current->children) {
        PrintNode(child, depth + 1);
    }
}