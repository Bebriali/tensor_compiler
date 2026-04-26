#pragma once
#include "node.hpp"
#include <string>

class TreeBuilder {
public:
    // Takes the head of the linear chain from Lexer
    explicit TreeBuilder(Node_t* tokens_root) : current_(tokens_head) {}

    // Main entry point to build the AST
    Node_t* parse();

private:
    Node_t* current_; // Current token in the linear chain

    // Helper: returns current token without consuming
    Node_t* peek() const { return current_; }

    // Helper: moves to the next token in the chain
    Node_t* consume();

    // Helper: checks if current token is a specific operator string
    bool matchOp(const std::string& op_str);

    // Error handling
    void syntaxError(const std::string& message);

    // Parsing rules (Recursive Descent)
    Node_t* parseExpression();   // Handles + and -
    Node_t* parseTerm();         // Handles * and /
    Node_t* parsePrimary();      // Handles NUM, ID, or (Expr)
    Node_t* parseAssignment();   // Handles var = expr;
};