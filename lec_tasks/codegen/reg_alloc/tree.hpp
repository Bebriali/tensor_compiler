#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "node.hpp"
#include "stack.hpp"

class Tree_t
{
    public:
        std::vector<std::shared_ptr<Node_t>> nodes;  // all nodes in the graph
        std::string name;
        Stack_t stack;

        explicit Tree_t(std::string n, size_t num_registers = 3)
            : name(std::move(n)), stack(num_registers) {}
        ~Tree_t() = default;

        void AddNode(std::shared_ptr<Node_t> node) {
            nodes.push_back(node);
        }

        void AddEdge(std::shared_ptr<Node_t> a, std::shared_ptr<Node_t> b) {
            a->neighbors.push_back(b);
            b->neighbors.push_back(a);
        }

        /**
        * @brief Performs graph coloring for register allocation
        */
        bool ColorGraph(size_t K = 3) {
            // Simplification phase
            FillStack(K);

            // Coloring phase
            return FillTree(K);
        }

        /**
        * @brief recursive dump of tree to console
        */
        void PrintNode(std::shared_ptr<Node_t> current = nullptr, int depth = 0) const {
            if (!current) {
                for (auto& node : nodes) {
                    std::cout << std::string(depth, ' ') << node->var << " : " << static_cast<int>(node->col) << std::endl;
                    for (auto& neigh : node->neighbors) {
                        if (auto n = neigh.lock()) {
                            PrintNode(n, depth + 1);
                        }
                    }
                }
            } else {
                std::cout << std::string(depth, ' ') << current->var << " : " << static_cast<int>(current->col) << std::endl;
            }
        }

        /**
        * @brief Считает общее количество узлов в дереве
        */
        size_t GetSize(std::shared_ptr<Node_t> current = nullptr) const {
            return nodes.size();
        }

        void FillStack(size_t K);
        bool FillTree(size_t K);

    private:
        /**
         * @brief находит узел с наибольшим, но меньшим N количеством связей
         */
        std::shared_ptr<Node_t> GetNodeN(size_t N);
};
