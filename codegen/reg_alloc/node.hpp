#pragma once

#include <memory>
#include <string>
#include <vector>

class Node_t
{
    public:
        enum class Color
        {
            NONE,
            RED,
            BLUE,
            YELLOW
        };

        std::string var;
        Color col;

        std::vector<std::weak_ptr<Node_t>> neighbors;  // interfering variables

        explicit Node_t(std::string v, Color c = Color::NONE)
            : var(std::move(v)), col(c) {}
        ~Node_t() = default;

        void SetColor(Color c) { col = c; }
        size_t Degree() const { return neighbors.size(); }
};
