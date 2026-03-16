#include "tree.hpp"
#include <iostream>

int main() {
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

    if (graph.ColorGraph(3)) {
        std::cout << "Coloring successful!" << std::endl;
    } else {
        std::cout << "Spill occurred." << std::endl;
    }

    graph.PrintNode();

    return 0;
}
