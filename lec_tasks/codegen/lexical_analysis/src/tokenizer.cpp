#include <cctype>
#include <iostream>

#include "tokenizer.hpp"

void Tokenizer::skipSpaces() {
    while (cursor_ < source_.length() && std::isspace(source_[cursor_])) {
        cursor_++;
    }
}

int Tokenizer::readNumber() {
    int res = 0;
    while (cursor_ < source_.length() && std::isdigit(source_[cursor_])) {
        res = res * 10 + (source_[cursor_++] - '0');
    }
    return res;
}

std::string Tokenizer::readWord() {
    std::string word;
    while (cursor_ < source_.length() && std::isalnum(source_[cursor_])) {
        word += source_[cursor_++];
    }
    return word;
}

std::vector<Node_t> Tokenizer::tokenize() {
    std::vector<Node_t> tokens;

    while (cursor_ < source_.length()) {
        skipSpaces();
        if (cursor_ >= source_.length()) break;

        char current = source_[cursor_];

        // eot
        if (current == '$') {
            tokens.emplace_back(TokenType::EOT, "$");
            cursor_++;
            break;
        }

        // nums
        if (std::isdigit(current)) {
            tokens.emplace_back(TokenType::NUM, readNumber());
        }
        // variables and operators
        else if (std::isalpha(current)) {
            std::string word = readWord();
            tokens.emplace_back(TokenType::VAR, word);
        }
        // op-s and symbols
        else {
            std::string op(1, source_[cursor_++]);
            tokens.emplace_back(TokenType::OP, op);
        }
    }
    return tokens;
}

Token_t Tokenizer::getNextToken() {
    skipSpaces();

    if (cursor_ >= source_.length()) {
        return Token_t(TokenType::EOT, "$");
    }

    char current = source_[cursor_];

    // id-s and key-words (int, phi, br, return, bb0)
    if (std::isalpha(current) || current == '_') {
        std::string word = readWord();

        // check for reserved words
        if (word == "phi")    return Token_t(TokenType::PHI, word);
        if (word == "br")     return Token_t(TokenType::BR, word);
        if (word == "return") return Token_t(TokenType::RETURN, word);
        if (word == "int")    return Token_t(TokenType::FUNC, word); // тип данных

        // else id (var or block name)
        return Token_t(TokenType::ID, word);
    }

    // NUM
    if (std::isdigit(current)) {
        return Token_t(TokenType::NUM, std::to_string(readNumber()));
    }

    // OP, L_CURL, etc. (punctuation)
    cursor_++;
    switch (current) {
        case '{': return Token_t(TokenType::L_CURL, "{");
        case '}': return Token_t(TokenType::R_CURL, "}");
        case '(': return Token_t(TokenType::L_PAREN, "(");
        case ')': return Token_t(TokenType::R_PAREN, ")");
        case ',': return Token_t(TokenType::COMMA, ",");
        case ':': return Token_t(TokenType::OP, ":");
        case ';': return Token_t(TokenType::OP, ";");
        case '*': return Token_t(TokenType::OP, "*");
        case '+': return Token_t(TokenType::OP, "+");
        case '-': return Token_t(TokenType::OP, "-");
        case '%': return Token_t(TokenType::OP, "%");

        case '=':
            if (cursor_ < source_.length() && source_[cursor_] == '=') {
                cursor_++;
                return Token_t(TokenType::OP, "==");
            }
            return Token_t(TokenType::OP, "=");

        case '<':
            if (cursor_ < source_.length() && source_[cursor_] == '=') {
                cursor_++;
                return Token_t(TokenType::OP, "<=");
            }
            return Token_t(TokenType::OP, "<");

        case '$': return Token_t(TokenType::EOT, "$");

        default:
            // unknown symbol (to add exception or stay with ERR_T)
            return Token_t(TokenType::ERR_T, std::string(1, current));
    }
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::FUNC:   return "FUNC";
        case TokenType::ID:     return "ID";
        case TokenType::BLOCK:  return "BLOCK";
        case TokenType::OP:     return "OP";
        case TokenType::NUM:    return "NUM";
        case TokenType::PHI:    return "PHI";
        case TokenType::BR:     return "BR";
        case TokenType::RETURN: return "RETURN";
        case TokenType::L_CURL: return "L_CURL";
        case TokenType::R_CURL: return "R_CURL";
        case TokenType::COMMA:  return "COMMA";
        case TokenType::EOT:    return "EOT";
        default:                return "UNKNOWN";
    }
}

void Tokenizer::dumpToken(std::vector<Node_t>& tokens, Dumpstream type) {
    switch(type) {
        case Dumpstream::CONSOLE:
            dumpToStream(tokens, std::cout, type);
            break;
        case Dumpstream::GRAPHVIZ:
        {
            // under comments for univ path to dump
            // std::string full_path = std::string(PROJECT_DUMP_DIR) + "/tokens.dot";
            std::string full_path = "token.dot";
            std::ofstream file(full_path);

            if (!file.is_open()) {
                throw std::runtime_error("Could not create dump in: " + full_path);
            }

            dumpToStream(tokens, file, type);
            break;
        }
    }

    return ;
}

void Tokenizer::dumpToStream(const std::vector<Node_t>& tokens, std::ostream& out, Dumpstream type) {
    switch(type) {
        case Dumpstream::GRAPHVIZ:
            out << "digraph G {\n  rankdir=LR;\n  node [shape=box];\n";
            break;
        case Dumpstream::CONSOLE:
            out << "=== TOKEN DUMP ===\n";
            break;
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        switch(type) {
            case Dumpstream::GRAPHVIZ:
                out << "  t" << i << " [label=\"" << tokenTypeToString(token.type) << "\\n'" << token.value << "'\"];\n";
                if (i + 1 < tokens.size()) {
                    out << "  t" << i << " -> t" << (i + 1) << ";\n";
                }
                break;

            case Dumpstream::CONSOLE:
                out << std::setw(3) << i << ": [" << tokenTypeToString(token.type) << "] '" << token.value << "'\n";
                break;
        }
    }

    switch(type) {
        case Dumpstream::GRAPHVIZ:
            out << "}\n";
            break;
        case Dumpstream::CONSOLE:
            out << "==================\n";
            break;
    }
}
