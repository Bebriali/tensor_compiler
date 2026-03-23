#include <iostream>

#include "TreeBuilder.hpp"

const Node_t& TreeBuilder::peek() const {
    if (pos_ >= tokens_.size()) return tokens_.back(); // the last is EOT
    return tokens_[pos_];
}

const Node_t& TreeBuilder::consume() {
    const Node_t& t = peek();
    if (pos_ < tokens_.size()) pos_++;
    return t;
}

bool TreeBuilder::matchOp(Operation op) {
    if (peek().type == TokenType::OP && std::get<Operation>(peek().data) == op) {
        consume();
        return true;
    }
    return false;
}

void TreeBuilder::syntaxError(const std::string& message) {
    std::cerr << "[Syntax Error] Position " << pos_ << ": " << message << std::endl;
    // to add exception for correct parsing
    throw std::runtime_error("Parsing failed");
}

Node_t* TreeBuilder::getG() {
    Node_t* root = getOperator();

    while (matchOp(Operation::SEM)) {
        if (peek().type == TokenType::EOT) break;
        Node_t* nextOp = getOperator();
        if (nextOp) root = Node_t::createOp(Operation::SEM, root, nextOp);
    }

    if (!matchOp(Operation::EOT)) {
        syntaxError("Expected end of file ($)");
    }
    return root;
}

Node_t* TreeBuilder::getConditional() {
    auto current = peek();
    if (current.type != TokenType::OP) return nullptr;

    Operation op = std::get<Operation>(current.data);
    if (op != Operation::IF && op != Operation::WHILE) return nullptr;

    // go through if/while
    consume();
    if (!matchOp(Operation::L_PR_EXP)) syntaxError("Expected '('");
    Node_t* cond = getLogicalExpr();
    if (!matchOp(Operation::R_PR_EXP)) syntaxError("Expected ')'");

    if (!matchOp(Operation::L_CURL)) syntaxError("Expected '{'");
    Node_t* body = getOperator();
    if (!matchOp(Operation::R_CURL)) syntaxError("Expected '}'");

    return Node_t::createOp(op, cond, body);
}

Node_t* TreeBuilder::getAssignment() {
    Node_t* var = getVariable();

    if (!matchOp(Operation::AST)) {
        syntaxError("Expected '='");
    }

    Node_t* expr = getAddSub();

    if (!matchOp(Operation::SEM)) {
        syntaxError("Expected ';'");
    }

    return Node_t::createOp(Operation::AST, var, expr);
}

Node_t* TreeBuilder::getAddSub() {
    Node_t* val = getMulDiv();

    while (peek().type == TokenType::OP) {
        Operation op = std::get<Operation>(peek().data);
        if (op == Operation::ADD || op == Operation::SUB) {
            consume();
            Node_t* nextVal = getMulDiv();
            val = Node_t::createOp(op, val, nextVal);
        } else break;
    }
    return val;
}

Node_t* TreeBuilder::getVariable() {
    if (peek().type == TokenType::VAR) {
        Node_t* node = new Node_t{TokenType::VAR};
        node->data = consume().data;
        return node;
    }
    syntaxError("Expected variable identifier");
    return nullptr;
}
