#pragma once
#include <vector>
#include <string>
#include <stdexcept>

#include "node.hpp"
#include "tokenizer.hpp"

struct Node_t {
    TokenType type;
    Node_t *left = nullptr;
    Node_t *right = nullptr;

    static Node_t* createOp(Operation op, Node_t* l = nullptr, Node_t* r = nullptr);
};

class TreeBuilder {
public:
    explicit TreeBuilder(const std::vector<Node_t>& tokens)
        : tokens_(tokens), pos_(0) {}

    Node_t* parse() { return getG(); }

private:
    const std::vector<Node_t>& tokens_;
    size_t pos_;

    Node_t* getG();            // Grammar root
    Node_t* getOperator();     // Statement or Block
    Node_t* getConditional();  // if / while
    Node_t* getAssignment();   // var = expr;
    Node_t* getLogicalExpr();  // a < b
    Node_t* getAddSub();       // + -
    Node_t* getMulDiv();       // * /
    Node_t* getPow();          // ^
    Node_t* getOperation();    // sin, cos, etc.
    Node_t* getPrExp();        // ( expr ) or base
    Node_t* getNumber();       // Const
    Node_t* getVariable();     // Var ID

    // utils
    const Node_t& peek() const;
    const Node_t& consume();
    bool matchOp(Operation op);
    void syntaxError(const std::string& message);
};
