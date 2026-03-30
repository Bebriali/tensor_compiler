#include "TreeBuilder.hpp"
#include <stdexcept>

// Moves to the next token using the first child (the chain logic)
Node_t* TreeBuilder::consume() {
    Node_t* temp = current_;
    if (current_ && !current_->children.empty()) {
        current_ = current_->children[0]; // Next token is the child in a chain
    } else {
        current_ = nullptr; // End of chain
    }
    return temp;
}

bool TreeBuilder::matchOp(const std::string& op_str) {
    if (current_ && current_->type == TokenType::OP) {
        if (std::get<std::string>(current_->data) == op_str) {
            consume();
            return true;
        }
    }
    return false;
}

void TreeBuilder::syntaxError(const std::string& message) {
    int line = current_ ? current_->line : -1;
    std::cerr << "[Syntax Error] Line " << line << ": " << message << std::endl;
    throw std::runtime_error("Parsing failed: " + message);
}

// Example: Parsing an assignment like "res0 = a + b;"
Node_t* TreeBuilder::parseAssignment() {
    if (!current_ || current_->type != TokenType::ID) {
        syntaxError("Expected identifier at start of assignment");
    }

    Node_t* varNode = consume(); // Get the variable ID
    varNode->children.clear();   // Disconnect from the lexer chain

    if (!matchOp("=")) {
        syntaxError("Expected '=' after variable");
    }

    Node_t* expr = parseExpression(); // Get the RHS expression

    if (!matchOp(";")) {
        syntaxError("Expected ';' after expression");
    }

    // Create a new branch node for the assignment
    Node_t* assignRoot = new Node_t(TokenType::OP, "=");
    assignRoot->addChild(varNode);
    assignRoot->addChild(expr);

    return assignRoot;
}

// Binary Operation Rule: Expr -> Term { (+|-) Term }
Node_t* TreeBuilder::parseExpression() {
    Node_t* left = parseTerm();

    while (current_ && current_->type == TokenType::OP) {
        std::string op = std::get<std::string>(current_->data);
        if (op == "+" || op == "-") {
            consume();
            Node_t* right = parseTerm();

            Node_t* opNode = new Node_t(TokenType::OP, op);
            opNode->addChild(left);
            opNode->addChild(right);
            left = opNode;
        } else break;
    }
    return left;
}

// Base Case: NUM, ID or ( Expression )
Node_t* TreeBuilder::parsePrimary() {
    if (!current_) syntaxError("Unexpected end of input");

    if (current_->type == TokenType::NUM || current_->type == TokenType::ID) {
        Node_t* val = consume();
        val->children.clear(); // Important: detach from lexer chain
        return val;
    }

    if (matchOp("(")) {
        Node_t* expr = parseExpression();
        if (!matchOp(")")) syntaxError("Expected ')'");
        return expr;
    }

    syntaxError("Expected number, variable or '('");
    return nullptr;
}

Node_t* TreeBuilder::parseTerm() {
    Node_t* left = parsePrimary();
    // Same logic as parseExpression but for '*' and '/'
    return left;
}

Tree_t TreeBuilder::parse() {
    // 1. Determine function name (optional logic)
    std::string funcName = "anonymous_scope";
    if (peek() && peek()->type == TokenType::FUNC) {
        // Logic to grab name...
    }

    // 2. Build the actual node hierarchy
    Node_t* astRoot = parseAssignment(); // Or parseBlock()

    // 3. Return the Tree_t object
    // Note: Tree_t takes ownership of astRoot
    return Tree_t(astRoot, funcName);
}