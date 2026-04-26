#include "tree.hpp"
#include <algorithm>
#include <set>

void Tree_t::FillStack(size_t K) {
    std::vector<std::shared_ptr<Node_t>> remaining = nodes;
    bool changed = true;
    while (changed && !remaining.empty()) {
        changed = false;
        for (auto it = remaining.begin(); it != remaining.end(); ) {
            if ((*it)->Degree() < K) {
                stack.PushBack(*it);
                it = remaining.erase(it);
                changed = true;
            } else {
                ++it;
            }
        }
    }
    // If remaining, they are spilled, but for simplicity, assume all can be colored
}

bool Tree_t::FillTree(size_t K) {
    std::vector<Node_t::Color> colors = {Node_t::Color::RED, Node_t::Color::BLUE, Node_t::Color::YELLOW};
    while (!stack.IsEmpty()) {
        auto node = stack.PopBack();
        std::set<Node_t::Color> used;
        for (auto& neigh_weak : node->neighbors) {
            if (auto neigh = neigh_weak.lock()) {
                if (neigh->col != Node_t::Color::NONE) {
                    used.insert(neigh->col);
                }
            }
        }
        for (auto c : colors) {
            if (used.find(c) == used.end()) {
                node->SetColor(c);
                break;
            }
        }
        if (node->col == Node_t::Color::NONE) {
            // Spill, but for now, assign RED or something
            node->SetColor(Node_t::Color::RED);
            return false; // indicate spill
        }
    }
    return true;
}

std::shared_ptr<Node_t> Tree_t::GetNodeN(size_t N) {
    std::shared_ptr<Node_t> candidate = nullptr;
    size_t max_deg = 0;
    for (auto& node : nodes) {
        size_t deg = node->Degree();
        if (deg < N && deg > max_deg) {
            max_deg = deg;
            candidate = node;
        }
    }
    return candidate;
}
