#include <fstream>
#include <string>
#include <iostream>
#include "dump.hpp"

void OnnxDumper::DumpConsole(const Tree_t& tree) const
{
    std::cout
        << std::endl
        << "---------ONNX_TREE_DUMP---------"
        << tree.name
        << "----"
        << std::endl;

    if (tree.root)
    {
        DumpNodeConsole(tree.root, 0);
    }
    else
    {
        std::cout
            << "Empty tree"
            << std::endl;
    }

    std::cout
        << "-------------------------------"
        << std::endl;
}

void OnnxDumper::DumpNodeConsole(std::shared_ptr<Node_t> node, int depth) const
{
    if (node == nullptr) return;

    std::string indent(depth * 2, ' ');

    std::cout << indent << "|-- " << node->op_type << " [" << node->name << "]" << std::endl;

    for (const auto& [attr_name, attr_val] : node->attributes)
    {
        std::cout << indent << "    | attr: " << attr_name << " = ";
        attr_val.print();
        std::cout << std::endl;
    }

    for (const auto& child : node->children)
    {
        DumpNodeConsole(child, depth + 1);
    }
}

void OnnxDumper::DumpGraphViz(const Tree_t& tree, const std::string& output_path) const
{
    std::ofstream out(output_path);
    if (!out.is_open()) {
        std::cerr
            << "Error: Could not open file "
            << output_path
            << " for writing."
            << std::endl;

        return;
    }

    // header for *.dot file
    out << "digraph ONNX_Model {\n";
    out << "  node [shape=box, style=filled, color=lightblue];\n";
    out << "  label = \"" << tree.name << "\";\n\n";

    if (tree.root)
    {
        std::set<Node_t*> visited;

        visited.insert(tree.root.get());

        for (const auto& child : tree.root->children) {
            if (child) {
                DumpNodeDot(child, out, visited);
            }
        }
    }

    out << "}"
        << std::endl;

    out.close();

    std::cout
        << "GraphViz file generated: "
        << output_path
        << std::endl;
}

void OnnxDumper::DumpNodeDot(std::shared_ptr<Node_t> node, std::ofstream& out, std::set<Node_t*>& visited) const
{
    if (!node || visited.count(node.get()))return;
    visited.insert(node.get());

    std::string attr_str = "";
    for (const auto& [name, val] : node->attributes)
    {
        attr_str += "\\n" + name + "=";

        // output in single string
        // later to add : toString() в Attribute_t.
        // now by concatenating
        attr_str += val.toString();
    }

    // type, name, attrs for current node
    out << "  \"" << node->name << "\" [label=\""
        << node->op_type << "\\n(" << node->name << ")"
        << attr_str << "\"];\n";

    for (const auto& child : node->children)
    {
        if (child)
        {
            out << "  \"" << node->name << "\" -> \"" << child->name << "\";\n";
            DumpNodeDot(child, out, visited);
        }
    }
}
