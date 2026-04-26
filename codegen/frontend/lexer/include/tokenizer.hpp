#pragma once

#include <string>
#include <vector>
#include <cstddef>

#include "node.hpp"

class Tokenizer {
public:
    enum class Dumpstream {
        CONSOLE,
        GRAPHVIZ
    };

    explicit Tokenizer(std::string source) : source_(std::move(source)), cursor_(0) {}

    Node_t* tokenize();

    void dumpToken(Node_t* tokens_root, Dumpstream type);

private:
    std::string source_;
    size_t cursor_ = 0;

    void skipSpaces();
    Node_t* getNextToken();

    int readNumber();
    std::string readWord();
    std::string readSymbolSequence();
    std::string tokenTypeToString(TokenType type);
    void dumpToStream(Node_t* tokens_root, std::ostream& out, Dumpstream type);
};
