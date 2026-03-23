#include <gtest/gtest.h>
#include "tokenizer.hpp"

TEST(TokenizerTest, SimpleAssignment) {
    std::string code = "res0 = 42;";
    Tokenizer lexer(code);
    auto tokens = lexer.tokenize();

    // Проверяем количество (ID, OP, NUM, OP, EOT)
    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].type, TokenType::ID);
    EXPECT_EQ(tokens[0].value, "res0");

    EXPECT_EQ(tokens[1].type, TokenType::OP);
    EXPECT_EQ(tokens[1].value, "=");

    EXPECT_EQ(tokens[2].type, TokenType::NUM);
    EXPECT_EQ(tokens[2].value, "42");
}

TEST(TokenizerTest, PhiFunction) {
    std::string code = "i1 = phi(i0, i_inc)";
    Tokenizer lexer(code);
    auto tokens = lexer.tokenize();

    // Проверяем наличие токена PHI
    bool found_phi = false;
    for(const auto& t : tokens) {
        if(t.type == TokenType::PHI) found_phi = true;
    }
    EXPECT_TRUE(found_phi);
}
