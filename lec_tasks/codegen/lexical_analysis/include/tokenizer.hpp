#pragma once

#include <string>
#include <vector>
#include <cstddef>

#include "node.hpp"

class Tokenizer {
public:
    enum Dumpstream class {
        CONSOLE,
        GRAPHVIZ
    }

    explicit Tokenizer(std::string source) : source_(std::move(source)) {}

    std::vector<Node_t> tokenize();

    void dumpToken(std::vector<Node_t>& tokens, Dumpstream type);

private:
    std::string source_;
    size_t cursor_ = 0;

    void skipSpaces();
    Node_t getNextToken();

    int readNumber();
    std::string readWord();
    std::string readSymbolSequence();
    std::string tokenTypeToString(TokenType type);
};
