#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <onnx/onnx_pb.h>

#include "parser.hpp"
#include "tree.hpp"

std::unique_ptr<Tree_t> OnnxParser::Parse(const std::string& file_path)
{
    assert(!file_path.empty());

    onnx::ModelProto model;
    std::ifstream input(file_path, std::ios::in | std::ios::binary);
    if (!LoadModelProto(file_path, model))
    {
        return nullptr;
    }

    const auto& graph = model.graph();

    // 1. Мапа: Имя_Тензора -> Узел_Который_Его_Создал
    // Это поможет нам понять, кто для кого является родителем.
    std::unordered_map<std::string, std::shared_ptr<Node_t>> output_to_producer;

    // Список всех созданных узлов для второго прохода
    std::vector<std::shared_ptr<Node_t>> all_nodes;
    all_nodes.reserve(graph.node_size());

    // --- Первый проход: Создаем объекты узлов ---
    for (int i = 0; i < graph.node_size(); ++i)
    {
        const auto& n = graph.node(i);
        std::string name = n.name().empty() ? (n.op_type() + "_" + std::to_string(i)) : n.name();

        auto node = std::make_shared<Node_t>(name, n.op_type());

        // --- НОВОЕ: Парсим атрибуты ноды ---
        for (const auto& attr : n.attribute())
        {
            const std::string& attr_name = attr.name();

            if (attr.has_i())
            {
                node->setAttr(attr_name, (int64_t)attr.i());
            }
            else if (attr.has_f())
            {
                node->setAttr(attr_name, (float)attr.f());
            }
            else if (attr.has_s())
            {
                node->setAttr(attr_name, attr.s()); // String
            }
            else if (attr.ints_size() > 0)
            {
                std::vector<int64_t> vals;
                for (auto v : attr.ints()) vals.push_back(v);
                node->setAttr(attr_name, vals);
            }
            else if (attr.floats_size() > 0)
            {
                std::vector<float> vals;
                for (auto v : attr.floats()) vals.push_back(v);
                node->setAttr(attr_name, vals);
            }
        }
        // ------------------------------------

        all_nodes.push_back(node);

        // Регистрируем выходные тензоры
        for (const auto& out_name : n.output())
        {
                output_to_producer[out_name] = node;
        }
    }

    // --- Второй проход: Сшиваем связи ---
    for (int i = 0; i < graph.node_size(); ++i)
    {
        const auto& n = graph.node(i);
        auto current_node = all_nodes[i];

        // Смотрим на входы узла
        for (const auto& in_name : n.input())
        {
            // Если этот вход был создан другим узлом из нашего списка
            if (output_to_producer.find(in_name) != output_to_producer.end())
            {
                auto parent = output_to_producer[in_name];
                parent->AddChild(current_node);
                current_node->parents.push_back(parent); // Используем weak_ptr внутри
            }
        }
    }

    // --- Поиск корней ---
    // В ONNX может быть несколько входов, но мы создадим виртуальный корень "Model",
    // к которому привяжем все узлы, у которых нет родителей (начало графа).
    auto virtual_root = std::make_shared<Node_t>("model_root", "Graph");
    for (auto& node : all_nodes)
    {
        if (node->parents.empty())
        {
            virtual_root->AddChild(node);
        }
    }

    std::string tree_name = std::filesystem::path(file_path).filename().string();
    return std::make_unique<Tree_t>(virtual_root, tree_name);
}

bool OnnxParser::LoadModelProto(const std::string& file_path, onnx::ModelProto& model) const
{
    std::ifstream input(file_path, std::ios::in | std::ios::binary);
    if (!input)
    {
        std::cerr << "Cannot open file: " << file_path << std::endl;
        return false;
    }
    if (!model.ParseFromIstream(&input))
    {
        std::cerr << "Failed to parse ONNX proto." << std::endl;
        return false;
    }
    return true;
}
