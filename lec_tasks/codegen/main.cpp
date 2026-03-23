#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "tokenizer.hpp"
#include "tree_builder.hpp"

using std::string;

string ReadFileToString(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

FILE* OpenFile(const string filename, const string streamtype) {
    FILE* f = fopen(filename.c_str(), streamtype.c_str());
    if (!f) {
        perror("OpenFile failed");
        exit(1);
    }
    return f;
}

int main(int argc, char** argv) {
    // getting generic IR code
    string generic_code = ReadFileToString("../ir/generic/code.gir");

    // tokenizator
    Tokenizer lexer(generic_code);
    std::vector<Token_t> tokens;
    try {
        tokens = lexer.tokenize();
    } catch (const std::exception& e) {
        std::cerr << "Lexer error: " << e.what() << std::endl;
        return 1;
    }

    // watching dump
    lexer.dumpToken(tokens, Tokenizer::Dumpstream::CONSOLE);
    lexer.dumpToken(tokens, Tokenizer::Dumpstream::GRAPHVIZ);

    TreeBuilder parser(tokens);
    Node_t* root = nullptr;
    try {
        root = parser.parse();

        // --- DUMP SECTION ---
        std::cout << "\n=== Abstract Syntax Tree Dump ===\n" << std::endl;
        if (root) {
            root->Dump();
        } else {
            std::cout << "Tree is empty!" << std::endl;
        }
        std::cout << "\n=================================\n" << std::endl;

        // probable dump to txt
        std::ofstream log("tree_log.txt");

    } catch (const std::exception& e) {
        std::cerr << RED("FATAL ERROR: ") << e.what() << std::endl;
        return 1;
    }

//     // translation to asm x86-64
//     FILE* asm_file = OpenFile("filename.asm", "wb");
//
//     Translate(Node_t*, FILE*) should be realised in translation.cpp
//     Translate(root, asm_file);
//
//     fclose(asm_file);

    DeleteTree(root);

    return 0;
}
