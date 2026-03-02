#include <gtest/gtest.h>
#include "parser.hpp"
#include "dump.hpp"
#include "tree.hpp"

TEST(OnnxE2E, ParseSimpleLinearModel) {
    OnnxParser parser;

    auto tree = parser.Parse("models/model2.onnx");

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->name, "SingleLinearLayer");
    EXPECT_GE(tree->GetSize(), 1);

    auto root = tree->root;
    EXPECT_EQ(root->op_type, "Gemm");
}
TEST(OnnxE2E, DumperCreatesFile) {
    auto root = std::make_shared<Node_t>("in", "Input");
    Tree_t tree(root, "TestTree");
    OnnxDumper dumper;

    dumper.DumpGraphViz(tree, "test_output.dot");

    // Проверяем, что файл действительно создался
    std::ifstream f("test_output.dot");
    EXPECT_TRUE(f.good());
}
