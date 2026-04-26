#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "tokenizer.hpp"
// #include "tree_builder.hpp"

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
    // Check if the user provided a file path
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_file.gir>" << std::endl;
        return 1;
    }

    // Get file path from command line arguments
    std::string file_path = argv[1];

    // Read the source code
    std::string generic_code;
    try {
        generic_code = ReadFileToString(file_path);
    } catch (const std::exception& e) {
        std::cerr << "File error: " << e.what() << std::endl;
        return 1;
    }

    // Initialize Lexer with the loaded code
    Tokenizer lexer(generic_code);
    Node_t* tokens_root = nullptr;

    try {
        // Build the token chain (linear tree)
        tokens_root = lexer.tokenize();
    } catch (const std::exception& e) {
        std::cerr << "Lexer error: " << e.what() << std::endl;
        return 1;
    }

    // Perform dumps
    lexer.dumpToken(tokens_root, Tokenizer::Dumpstream::CONSOLE);
    lexer.dumpToken(tokens_root, Tokenizer::Dumpstream::GRAPHVIZ);

    // Clean up memory (assuming Node_t has a recursive destructor)
    delete tokens_root;

/** probable logit for tree building
*/
//    // ... lexer.tokenize() ...
//
//    TreeBuilder parser(tokens_chain);
//
//    // parser.parse() returns Tree_t by value
//    Tree_t myTree = parser.parse();
//
//    // Use the tree's dump method
//    myTree.PrintNode();
//
//    // When myTree goes out of scope, it automatically deletes the whole AST

    return 0;
}