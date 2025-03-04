#include "Expression.hpp"

// Конструктор для числа
template<typename T>
Expression<T>::Expression(T value) : root(std::make_unique<NumberNode>(value)) {}

// Конструктор для переменной
template<typename T>
Expression<T>::Expression(const std::string& variable) : root(std::make_unique<VariableNode>(variable)) {}

// Конструктор для узла
template<typename T>
Expression<T>::Expression(std::unique_ptr<Node> root) : root(std::move(root)) {}

// Конструктор копирования
template<typename T>
Expression<T>::Expression(const Expression& other) : root(cloneTree(other.root)) {}

// Конструктор перемещения
template<typename T>
Expression<T>::Expression(Expression&& other) noexcept : root(std::move(other.root)) {}

// Оператор копирования
template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        root = cloneTree(other.root);
    }
    return *this;
}

// Оператор перемещения
template<typename T>
Expression<T>& Expression<T>::operator=(Expression&& other) noexcept {
    if (this != &other) {
        root = std::move(other.root);
    }
    return *this;
}

// Операция сложения
template<typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    return Expression(std::make_unique<AdditionNode>(cloneTree(root), cloneTree(other.root)));
}

// Преобразование выражения в строку
template<typename T>
std::string Expression<T>::toString() const {
    return root ? root->toString() : "";
}

// Подстановка значения переменной
template<typename T>
T Expression<T>::evaluate(const std::string& variable, T value) const {
    std::unordered_map<std::string, T> variables = {{variable, value}};
    return evaluate(variables);
}

// Вычисление выражения при заданных значениях переменных
template<typename T>
T Expression<T>::evaluate(const std::unordered_map<std::string, T>& variables) const {
    return root ? root->evaluate(variables) : T();
}

// Символьное дифференцирование
template<typename T>
Expression<T> Expression<T>::diff(const std::string& variable) const {
    return Expression(root->diff(variable));
}

// Явное инстанцирование шаблона для типа double
template class Expression<double>;