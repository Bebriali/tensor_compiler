#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "tokenizer.hpp"

TEST(TokenizerTest, DumpToConsoleCheck) {
    Tokenizer lexer("res0 = 10;");
    auto tokens = lexer.tokenize();

    // 1. Сохраняем старый буфер cout
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();

    // 2. Создаем свой поток-ловушку
    std::ostringstream strCout;
    std::cout.rdbuf(strCout.rdbuf());

    // 3. Вызываем публичный метод
    lexer.dumpToken(tokens, Tokenizer::Dumpstream::CONSOLE);

    // 4. Возвращаем всё как было (КРИТИЧНО!)
    std::cout.rdbuf(oldCoutStreamBuf);

    // 5. Проверяем, что поймали в strCout
    std::string output = strCout.str();
    EXPECT_TRUE(output.find("res0") != std::string::npos);
}

TEST(DumpTest, GraphvizSyntaxValidity) {
    std::vector<Node_t> tokens = {{TokenType::ID, "x"}};
    std::stringstream buffer;
    Tokenizer lexer("");

    lexer.dumpToStream(tokens, buffer, Tokenizer::Dumpstream::GRAPHVIZ);
    std::string dot_code = buffer.str();

    // Минимальные требования к DOT-файлу
    EXPECT_EQ(dot_code.front(), 'd'); // начинается с "digraph"
    EXPECT_TRUE(dot_code.find("{") != std::string::npos);
    EXPECT_TRUE(dot_code.find("}") != std::string::npos);

    // Проверка баланса скобок
    int open_braces = std::count(dot_code.begin(), dot_code.end(), '{');
    int close_braces = std::count(dot_code.begin(), dot_code.end(), '}');
    EXPECT_EQ(open_braces, close_braces);
}

TEST(DumpTest, DeepTreeNoStackOverflow) {
    // Создаем "длинную" ветку дерева: 1 + 1 + 1 + ...
    Node_t* root = new Node_t(TokenType::OP, Operation::ADD);
    Node_t* current = root;
    for(int i = 0; i < 100; ++i) {
        current->left = new Node_t(TokenType::NUM, 1);
        current->right = new Node_t(TokenType::OP, Operation::ADD);
        current = current->right;
    }
    current->left = new Node_t(TokenType::NUM, 1);

    // Проверяем, что дамп проходит без сегфолта
    std::stringstream ss;
    ASSERT_NO_THROW({
        // Допустим, у тебя есть метод dumpToStream для дерева
        root->DumpToStream(ss);
    });
}
