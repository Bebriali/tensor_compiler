#include <gtest/gtest.h>
#include "../src/tree/tree.hpp"

// testing node ctor
// тест создания узла
TEST(NodeTest, ConstructorSetsProperties)
{
    Node_t node("conv1", "conv");

    EXPECT_EQ(node.name, "conv1");
    EXPECT_EQ(node.op_type, "conv");
    EXPECT_TRUE(node.children.empty());
}

// testing node connection
// тест связей между узлами
TEST(NodeTest, AddChildIncreasesCount)
{
    auto parent = std::make_shared<Node_t> ("root", "Input");
    auto child = std::make_shared<Node_t>("node1", "Relu");

    parent->AddChild(child);

    ASSERT_EQ(parent->children.size(), 1);
    EXPECT_EQ(parent->children[0]->name, "node1");
}

// testing tree with a cycle
// тест дерева циклом
TEST(TreeTest, CalculateCorrectSize)
{
    auto r = std::make_shared<Node_t>("in", "In");
    auto c1 = std::make_shared<Node_t>("n1", "op");
    auto c2 = std::make_shared<Node_t>("n2", "op");

    r->AddChild(c1);
    c1->AddChild(c2);

    Tree_t tree(r, "OnnxTree");

    EXPECT_EQ(tree.GetSize(), 3);
    EXPECT_EQ(tree.name, "OnnxTree");
}
