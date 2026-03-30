#include <cctype>
#include <iostream>
#include <fstream>
#include <iomanip> // for setw()
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
    while (cursor_ < source_.length() && (std::isalnum(source_[cursor_]) || source_[cursor_] == '_')) {
        word += source_[cursor_++];
    }
    return word;
}

Node_t* Tokenizer::tokenize() {
    Node_t* head = nullptr;
    Node_t* current = nullptr;

    while (true) {
        Node_t* newNode = getNextToken();

        if (!head) {
            head = newNode;
            current = head;
        } else {
            current->addChild(newNode);
            current = newNode;
        }

        if (newNode->type == TokenType::EOT || newNode->type == TokenType::ERR_T) {
            break;
        }
    }

    return head;
}

Node_t* Tokenizer::getNextToken() {
    skipSpaces();

    if (cursor_ >= source_.length()) {
        return new Node_t(TokenType::EOT, "$");
    }

    char current = source_[cursor_];

    // id's and key-words
    if (std::isalpha(current) || current == '_') {
        std::string word = readWord();

        if (word == "phi")    return new Node_t(TokenType::PHI, word);
        if (word == "br")     return new Node_t(TokenType::BR, word);
        if (word == "return") return new Node_t(TokenType::RETURN, word);
        if (word == "int")    return new Node_t(TokenType::FUNC, word);

        if (word.length() >= 3 && word[0] == 'b' && word[1] == 'b' && std::isdigit(word[2]))
                              return new Node_t(TokenType::BLOCK, word);

        return new Node_t(TokenType::ID, word);
    }

    // NUM
    if (std::isdigit(current)) {
        return new Node_t(TokenType::NUM, readNumber());
    }

    // operators and punctuation
    cursor_++;
    switch (current) {
        case '{': return new Node_t(TokenType::L_CURL, "{");
        case '}': return new Node_t(TokenType::R_CURL, "}");
        case '(': return new Node_t(TokenType::L_PAREN, "(");
        case ')': return new Node_t(TokenType::R_PAREN, ")");
        case ',': return new Node_t(TokenType::COMMA, ",");
        case ':': return new Node_t(TokenType::OP, ":");
        case ';': return new Node_t(TokenType::OP, ";");
        case '*': return new Node_t(TokenType::OP, "*");
        case '+': return new Node_t(TokenType::OP, "+");
        case '-': return new Node_t(TokenType::OP, "-");
        case '%': return new Node_t(TokenType::OP, "%");

        case '=':
            if (cursor_ < source_.length() && source_[cursor_] == '=') {
                cursor_++;
                return new Node_t(TokenType::OP, "==");
            }
            return new Node_t(TokenType::OP, "=");

        case '<':
            if (cursor_ < source_.length() && source_[cursor_] == '=') {
                cursor_++;
                return new Node_t(TokenType::OP, "<=");
            }
            return new Node_t(TokenType::OP, "<");

        case '>':
            if (cursor_ < source_.length() && source_[cursor_] == '=') {
                cursor_++;
                return new Node_t(TokenType::OP, ">=");
            }
            return new Node_t(TokenType::OP, ">");

        case '$': return new Node_t(TokenType::EOT, "$");

        default:
            // to emplace ERR_T we can throw std::runtime_error
            return new Node_t(TokenType::EOT, std::string(1, current));
    }
}

std::string Tokenizer::tokenTypeToString(TokenType type) {
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

void Tokenizer::dumpToken(Node_t* tokens_root, Dumpstream type) {
    switch(type) {
        case Dumpstream::CONSOLE:
            dumpToStream(tokens_root, std::cout, type);
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

            dumpToStream(tokens_root, file, type);
            break;
        }
    }

    return ;
}

void Tokenizer::dumpToStream(Node_t* tokens_root, std::ostream& out, Dumpstream type) {
    switch(type) {
        case Dumpstream::GRAPHVIZ:
            out << "digraph G {\n  rankdir=LR;\n  node [shape=box];\n";
            break;
        case Dumpstream::CONSOLE:
            out << "=== TOKEN DUMP ===\n";
            break;
    }

    const Node_t* current = tokens_root;
    int i = 0;

    // children[0] is the only place for next token
    while (current != nullptr) {
        // getting string value from variant for dump
        std::string val = std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return arg; // already string
            } else {
                return std::to_string(arg); // int or double to string
            }
        }, current->data);

        switch(type) {
            case Dumpstream::GRAPHVIZ:
                out << "  t" << i << " [label=\"" << tokenTypeToString(current->type)
                    << "\\n'" << val << "'\"];\n";

                // Если есть ребенок, рисуем стрелку к нему
                if (!current->children.empty() && current->children[0] != nullptr) {
                    out << "  t" << i << " -> t" << (i + 1) << ";\n";
                }
                break;

            case Dumpstream::CONSOLE:
                out << std::setw(3) << i << ": [" << tokenTypeToString(current->type)
                    << "] '" << val << "'\n";
                break;
        }

        // next token
        if (!current->children.empty()) {
            current = current->children[0];
            i++;
        } else {
            current = nullptr;
        }
    }

    switch(type) {
        case Dumpstream::GRAPHVIZ:
            out << "}\n";
            break;
        case Dumpstream::CONSOLE:
            out << "==========================\n";
            break;
    }
}
