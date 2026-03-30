#include <gtest/gtest.h>
#include "../../tree.hpp"

TEST(RegisterAllocationE2E, ColorSimpleGraph) {
    Tree_t graph("test");

    auto a = std::make_shared<Node_t>("a");
    auto b = std::make_shared<Node_t>("b");
    auto c = std::make_shared<Node_t>("c");
    auto d = std::make_shared<Node_t>("d");

    graph.AddNode(a);
    graph.AddNode(b);
    graph.AddNode(c);
    graph.AddNode(d);

    // a interferes with b and c
    graph.AddEdge(a, b);
    graph.AddEdge(a, c);

    // b interferes with d
    graph.AddEdge(b, d);

    // c interferes with d
    graph.AddEdge(c, d);

    bool success = graph.ColorGraph(3);

    EXPECT_TRUE(success);
    EXPECT_EQ(graph.name, "test");
    EXPECT_EQ(graph.GetSize(), 4);

    // Check that all nodes have colors assigned
    for (auto& node : graph.nodes) {
        EXPECT_NE(node->col, Node_t::Color::NONE);
    }

    // Check that interfering nodes have different colors
    EXPECT_NE(a->col, b->col);
    EXPECT_NE(a->col, c->col);
    EXPECT_NE(b->col, d->col);
    EXPECT_NE(c->col, d->col);
}

TEST(RegisterAllocationE2E, SpillWhenNotEnoughRegisters) {
    Tree_t graph("spill_test");

    auto a = std::make_shared<Node_t>("a");
    auto b = std::make_shared<Node_t>("b");
    auto c = std::make_shared<Node_t>("c");
    auto d = std::make_shared<Node_t>("d");

    graph.AddNode(a);
    graph.AddNode(b);
    graph.AddNode(c);
    graph.AddNode(d);

    // Create a clique: all interfere with each other
    graph.AddEdge(a, b);
    graph.AddEdge(a, c);
    graph.AddEdge(a, d);
    graph.AddEdge(b, c);
    graph.AddEdge(b, d);
    graph.AddEdge(c, d);

    bool success = graph.ColorGraph(3);

    EXPECT_FALSE(success);  // Should spill
    EXPECT_EQ(graph.name, "spill_test");
    EXPECT_EQ(graph.GetSize(), 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
