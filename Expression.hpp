#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

template<typename T>
class Expression {
public:
    // Конструкторы
    Expression(T value); // Конструктор для числа
    Expression(const std::string& variable); // Конструктор для переменной
    Expression(std::unique_ptr<typename Expression<T>::Node> root); // Конструктор для узла

    // Конструктор копирования
    Expression(const Expression& other);

    // Конструктор перемещения
    Expression(Expression&& other) noexcept;

    // Деструктор
    ~Expression() = default;

    // Оператор копирования
    Expression& operator=(const Expression& other);

    // Оператор перемещения
    Expression& operator=(Expression&& other) noexcept;

    // Операция сложения
    Expression operator+(const Expression& other) const;

    // Преобразование выражения в строку
    std::string toString() const;

    // Операция подстановки значения переменной
    T evaluate(const std::string& variable, T value) const;

    // Операция вычисления выражения при заданных значениях переменных
    T evaluate(const std::unordered_map<std::string, T>& variables) const;

private:
    // Базовый класс для узлов выражения
    struct Node {
        virtual ~Node() = default;
        virtual T evaluate(const std::unordered_map<std::string, T>& variables) const = 0;
        virtual std::string toString() const = 0;
        virtual std::unique_ptr<Node> clone() const = 0;
    };

    // Узел для числа
    struct NumberNode : Node {
        T value;
        NumberNode(T value) : value(value) {}
        T evaluate(const std::unordered_map<std::string, T>& variables) const override { return value; }
        std::string toString() const override { return std::to_string(value); }
        std::unique_ptr<Node> clone() const override { return std::make_unique<NumberNode>(value); }

    };

    // Узел для переменной
    struct VariableNode : Node {
        std::string variable;
        VariableNode(const std::string& variable) : variable(variable) {}
        T evaluate(const std::unordered_map<std::string, T>& variables) const override {
            auto it = variables.find(variable);
            if (it != variables.end()) return it->second;
            throw std::runtime_error("Variable not found: " + variable);
        }
        std::string toString() const override { return variable; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<VariableNode>(variable); }

    };

    // Узел для операции сложения
    struct AdditionNode : Node {
        std::unique_ptr<Node> left, right;
        AdditionNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
            : left(std::move(left)), right(std::move(right)) {}
        T evaluate(const std::unordered_map<std::string, T>& variables) const override {
            return left->evaluate(variables) + right->evaluate(variables);
        }
        std::string toString() const override {
            return "(" + left->toString() + " + " + right->toString() + ")";
        }
        std::unique_ptr<Node> clone() const override {
            return std::make_unique<AdditionNode>(left->clone(), right->clone());
        }

    };

    std::unique_ptr<Node> root; // Корень дерева выражения

    // Вспомогательная функция для клонирования дерева
    std::unique_ptr<Node> cloneTree(const std::unique_ptr<Node>& node) const {
        if (node) return node->clone();
        return nullptr;
    }
};

#endif // EXPRESSION_HPP