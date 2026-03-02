#include <gtest/gtest.h>
#include "tree.hpp"

TEST(TreeUnit, GetSizeAndEmpty) {
    // Non-empty tree
    auto root = std::make_shared<Node_t>("root", "RootOp");
    auto a = std::make_shared<Node_t>("a", "OpA");
    auto b = std::make_shared<Node_t>("b", "OpB");

    root->AddChild(a);
    root->AddChild(b);

    Tree_t t(root, "TreeTest");
    EXPECT_EQ(t.GetSize(), 3u);

    // Empty tree
    Tree_t empty(nullptr, "Empty");
    EXPECT_EQ(empty.GetSize(), 0u);
}
