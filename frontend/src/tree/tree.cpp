#include <iomanip>
#include <iostream>
#include <sstream>
#include "tree.hpp"
#include <utility>  // Needed for std::move


void Node_t::AddChild(std::shared_ptr<Node_t> child) {
    if (!child) return;

    children.push_back(child);
}

void Tree_t::PrintNode(std::shared_ptr<Node_t> current, int depth) const
{
    // Only use root if we are at the very start of the call
    if (!current) {
        if (depth == 0) current = root;
        if (!current) return;
    }

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    std::cout << "|-- " << current->op_type << " [" << current->name << "]" << std::endl;

    for (auto& child : current->children) {
        PrintNode(child, depth + 1);
    }
}

size_t Tree_t::GetSize(std::shared_ptr<Node_t> current) const
{
    // If called with nullptr, start from root.
    // If root is also null, size is 0.
    if (!current) {
        current = root;
        if (!current) return 0;
    }

    size_t count = 1;
    for (auto& child : current->children)
    {
        // Only recurse if child is not null to avoid the "reset to root" bug
        if (child) {
            count += GetSize(child);
        }
    }
    return count;
}

std::string Attribute_t::toString() const
{
    std::ostringstream oss;
    switch (type) {
        case Type::INT:    oss << asInt(); break;
        case Type::FLT:    oss << std::fixed << std::setprecision(2) << asFlt(); break;
        case Type::STR:    oss << asStr(); break;
        case Type::VECINT: {
            oss << "[";
            const auto& vec = asVecInt();
            for (size_t i = 0; i < vec.size(); ++i) {
                oss << vec[i] << (i == vec.size() - 1 ? "" : ",");
            }
            oss << "]";
            break;
        }
        case Type::VECFLT: {
            oss << "[";
            const auto& vec = asVecFlt();
            for (size_t i = 0; i < vec.size(); ++i) {
                oss << vec[i] << (i == vec.size() - 1 ? "" : ",");
            }
            oss << "]";
            break;
        }
    }
    return oss.str();
}
