#pragma once

#include <variant>
#include <string>
#include <vector>
#include <iostream>

enum class TokenType {
    FUNC,   // int, void, float
    BLOCK,  // bb0, bb1
    ID,     // res0, a, n
    OP,     // =, +, *, <, :
    NUM,    // 0, 1, 2
    BR,     // br
    PHI,    // phi
    RETURN, // return
    L_CURL, R_CURL,
    L_PAREN, R_PAREN,
    COMMA,
    EOT,
    ERR_T
};

struct Node_t {
    TokenType type;
    std::variant<int, double, std::string> data;
    int line;

    std::vector<Node_t*> children;

    Node_t(TokenType t, std::variant<int, double, std::string> d, int l = 0)
        : type(t), data(std::move(d)), line(l) {}

    void addChild(Node_t* child) {
        if (child) children.push_back(child);
    }

    void Dump(int indent = 0) const;
};

inline void Node_t::Dump(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << "  ";

    std::cout << "|-- ";
    switch (type) {
        case TokenType::NUM:
            if (std::holds_alternative<int>(data)) std::cout << "NUM: " << std::get<int>(data);
            else std::cout << "NUM (dbl): " << std::get<double>(data);
            break;
        case TokenType::ID:     std::cout << "ID: " << std::get<std::string>(data); break;
        case TokenType::BLOCK:  std::cout << "BLOCK: " << std::get<std::string>(data); break;
        case TokenType::OP:     std::cout << "OP: " << std::get<std::string>(data); break;
        case TokenType::FUNC:   std::cout << "FUNC: " << std::get<std::string>(data); break;
        case TokenType::BR:     std::cout << "BR"; break;
        case TokenType::PHI:    std::cout << "PHI"; break;
        case TokenType::RETURN: std::cout << "RETURN"; break;
        default:                std::cout << "TOKEN (" << (int)type << ")"; break;
    }
    std::cout << std::endl;

    for (Node_t* child : children) {
        if (child) {
            child->Dump(indent + 1);
        }
    }
}
