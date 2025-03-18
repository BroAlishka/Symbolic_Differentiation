#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <variant>
#include <string>
#include <map>
#include <memory>
#include <cmath>
#include <iostream>
#include <complex>
#include <vector>
#include <stack>

template <typename T>
class Expression {
public:
    // Типы выражений
    struct Number {
        T value;
    };

    struct Variable {
        std::string name;
    };

    struct OperationAdd {
        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Expression<T>> right;
    };

    struct OperationSub {
        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Expression<T>> right;
    };

    struct OperationMul {
        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Expression<T>> right;
    };

    struct OperationDiv {
        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Expression<T>> right;
    };

    struct OperationPow {
        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Expression<T>> right;
    };

    struct OperationSin {
        std::shared_ptr<Expression<T>> arg;
    };

    struct OperationCos {
        std::shared_ptr<Expression<T>> arg;
    };

    struct OperationLn {
        std::shared_ptr<Expression<T>> arg;
    };

    struct OperationExp {
        std::shared_ptr<Expression<T>> arg;
    };

    // Тип, который может хранить любое выражение
    using ExprVariant = std::variant<
        Number, Variable,
        OperationAdd, OperationSub, OperationMul, OperationDiv, OperationPow,
        OperationSin, OperationCos, OperationLn, OperationExp
    >;

    // Конструкторы
    Expression(T value) : expr_(Number{value}) {}
    Expression(std::string name) : expr_(Variable{name}) {}
    Expression(ExprVariant expr) : expr_(expr) {}

    // Конструктор копирования
    Expression(const Expression& other) : expr_(other.expr_) {}

    Expression(Expression&& other) noexcept : expr_(std::move(other.expr_)) {}

    Expression& operator=(const Expression& other) {
        if (this != &other) {
            expr_ = other.expr_;
        }
        return *this;
    }

    Expression& operator=(Expression&& other) noexcept {
        if (this != &other) {
            expr_ = std::move(other.expr_);
        }
        return *this;
    }

    ~Expression() = default;

    // Вычисление выражения
    T eval(std::map<std::string, T> context) const;

    // Преобразование в строку
    std::string to_string() const;

    // Перегрузки операторов
    Expression operator+(const Expression& other) const;
    Expression operator-(const Expression& other) const;
    Expression operator*(const Expression& other) const;
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;

    // Функции
    Expression sin() const;
    Expression cos() const;
    Expression ln() const;
    Expression exp() const;

    // Подстановка значения переменной
    Expression substitute(const std::string& varName, const T& value) const;

    // Создание выражения из строки

    // Дифференцирование по переменной
    Expression diff(const std::string& varName) const;

private:
    ExprVariant expr_;
};

// Функции для создания выражений
template <typename T>
Expression<T> make_value(T value) {
    return Expression<T>(value);
}

template <typename T>
Expression<T> make_variable(std::string name) {
    return Expression<T>(name);
}

#endif // EXPRESSION_HPP