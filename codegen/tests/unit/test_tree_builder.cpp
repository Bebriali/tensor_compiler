#include <gtest/gtest.h>
#include "tree_builder.hpp"

TEST(TreeBuilderTest, ParseAddition) {
    // Подсовываем заранее известные токены (имитация работы лексера)
    std::vector<Token_t> tokens = {
        {TokenType::NUM, "10"},
        {TokenType::OP,  "+"},
        {TokenType::NUM, "20"},
        {TokenType::OP,  ";"},
        {TokenType::EOT, "$"}
    };

    TreeBuilder parser(tokens);
    Node_t* root = parser.parse();

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(std::get<Operation>(root->data), Operation::ADD);

    ASSERT_NE(root->left, nullptr);
    EXPECT_EQ(root->left->type, TokenType::NUM);
    EXPECT_EQ(std::get<int>(root->left->data), 10);

    ASSERT_NE(root->right, nullptr);
    EXPECT_EQ(root->right->type, TokenType::NUM);
    EXPECT_EQ(std::get<int>(root->right->data), 20);
}
