#pragma once

#include <set>
#include <string>
#include <fstream>

#include "tree.hpp"


class OnnxDumper
{
    public:
        void DumpConsole(const Tree_t& tree) const;
        void DumpGraphViz(const Tree_t& tree, const std::string& output_path) const;
    private:
        void DumpNodeConsole(std::shared_ptr<Node_t> node, int depth) const;
        void DumpNodeDot(std::shared_ptr<Node_t> node, std::ofstream& out, std::set<Node_t*>& visited) const;
};
