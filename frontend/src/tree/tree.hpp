#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

class Attribute_t
{
    public:
        enum class Type
        {
            INT,
            FLT,
            STR,
            VECINT,
            VECFLT
        };

        using Value = std::variant
        <
            int64_t,
            float,
            std::string,
            std::vector<int64_t>,
            std::vector<float>
        >;

        Attribute_t() = default;
        Attribute_t(Value val, Type t) : data(std::move(val)), type(t) {}

        /**
        * getters
        */
        int64_t                 asInt()     const { return std::get<int64_t>(data);             }
        float                   asFlt()     const { return std::get<float>(data);               }
        std::string             asStr()     const { return std::get<std::string>(data);         }
        std::vector<int64_t>    asVecInt()  const { return std::get<std::vector<int64_t>>(data);}
        std::vector<float>      asVecFlt()  const { return std::get<std::vector<float>>(data);  }

        Type getType() const { return type; }
        std::string toString() const ;

        void print() const
        {
            switch (type) {
                case Type::INT:     std::cout << asInt(); break;
                case Type::FLT:     std::cout << asFlt(); break;
                case Type::STR:     std::cout << "\"" << asStr() << "\""; break;
                case Type::VECINT:  PrintVec(asVecInt()); break;
                case Type::VECFLT:  PrintVec(asVecFlt()); break;
            }
        }
    private:
        Value data;
        Type type;

        template <typename T>
        void PrintVec(const std::vector<T>& vec) const
        {
            std::cout << "[";
            for (size_t i = 0; i < vec.size(); i++)
            {
                std::cout
                    << vec[i]
                    << (i == vec.size() - 1) ? "" : ", ";
            }
            std::cout << "]";
        }
};

class Node_t
{
    public:
        std::string name;
        std::string op_type;

        std::map<std::string, Attribute_t> attributes;

        std::vector<std::shared_ptr<Node_t>> children;
        // Используем weak_ptr, чтобы избежать циклических ссылок (Memory Leak)
        std::vector<std::weak_ptr<Node_t>> parents;

        void setAttr(std::string name, int64_t val)
        {
            attributes[name] = Attribute_t(val, Attribute_t::Type::INT);
        }
        void setAttr(std::string name, std::vector<int64_t> val)
        {
            attributes[name] = Attribute_t(val, Attribute_t::Type::VECINT);
        }
        void setAttr(std::string name, float val)
        {
            attributes[name] = Attribute_t(val, Attribute_t::Type::FLT);
        }
        void setAttr(std::string name, std::vector<float> val)
        {
            attributes[name] = Attribute_t(val, Attribute_t::Type::VECFLT);
        }
        void setAttr(std::string name, std::string val)
        {
            attributes[name] = Attribute_t(val, Attribute_t::Type::STR);
        }

        explicit Node_t(std::string n, std::string op)
            : name(std::move(n)), op_type(std::move(op)) {}

        /**
        * @brief Добавляет ребенка и автоматически устанавливает текущий узел как родителя.
        * Мы используем shared_from_this, но для простоты в этом классе
        * можно передавать родителя извне или через указатель.
        */
        void AddChild(std::shared_ptr<Node_t> child);
};

class Tree_t
{
    public:
        std::shared_ptr<Node_t> root;
        std::string name;

        // В C++ лучше инициализировать переменные сразу
        int size = 0;

        explicit Tree_t(std::shared_ptr<Node_t> r, std::string n)
            : root(std::move(r)), name(std::move(n)) {}

        /**
        * @brief Рекурсивный вывод дерева в консоль
        */
        void PrintNode(std::shared_ptr<Node_t> current = nullptr, int depth = 0) const;

        /**
        * @brief Считает общее количество узлов в дереве
        */
        size_t GetSize(std::shared_ptr<Node_t> current = nullptr) const;
};
