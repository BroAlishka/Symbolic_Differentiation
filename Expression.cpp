#include "Expression.hpp"
#include <sstream>
#include <iomanip>

template <typename T>
T Expression<T>::eval(std::map<std::string, T> context) const {
    return std::visit([&](auto&& arg) -> T {
        using U = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<U, Number>) {
            return arg.value;
        } else if constexpr (std::is_same_v<U, Variable>) {
            if (context.find(arg.name) != context.end()) {
                return context.at(arg.name);
            } 
        } else if constexpr (std::is_same_v<U, OperationAdd>) {
            return arg.left->eval(context) + arg.right->eval(context);
        } else if constexpr (std::is_same_v<U, OperationSub>) {
            return arg.left->eval(context) - arg.right->eval(context);
        } else if constexpr (std::is_same_v<U, OperationMul>) {
            return arg.left->eval(context) * arg.right->eval(context);
        } else if constexpr (std::is_same_v<U, OperationDiv>) {
            return arg.left->eval(context) / arg.right->eval(context);
        } else if constexpr (std::is_same_v<U, OperationPow>) {
            return std::pow(arg.left->eval(context), arg.right->eval(context));
        } else if constexpr (std::is_same_v<U, OperationSin>) {
            return std::sin(arg.arg->eval(context));
        } else if constexpr (std::is_same_v<U, OperationCos>) {
            return std::cos(arg.arg->eval(context));
        } else if constexpr (std::is_same_v<U, OperationLn>) {
            return std::log(arg.arg->eval(context));
        } else if constexpr (std::is_same_v<U, OperationExp>) {
            return std::exp(arg.arg->eval(context));
        }
        return T{};
    }, expr_);
}
template <typename T>
Expression<T> Expression<T>::diff(const std::string& varName) const {
    return std::visit([&](auto&& arg) -> Expression<T> {
        using U = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<U, Number>) {
            // Производная константы
            return Expression<T>(T{0});
        } else if constexpr (std::is_same_v<U, Variable>) {
            // Производная переменной
            if (arg.name == varName) {
                return Expression<T>(T{1});
            } else {
                return Expression<T>(T{0});
            }
        } else if constexpr (std::is_same_v<U, OperationAdd>) {
            // Производная суммы
            return arg.left->diff(varName) + arg.right->diff(varName);
        } else if constexpr (std::is_same_v<U, OperationSub>) {
            // Производная разности
            return arg.left->diff(varName) - arg.right->diff(varName);
        } else if constexpr (std::is_same_v<U, OperationMul>) {
            // Производная произведения
            return (arg.left->diff(varName) * *arg.right) + (*arg.left * arg.right->diff(varName));
        } else if constexpr (std::is_same_v<U, OperationDiv>) {
            // Производная частного
            return ((arg.left->diff(varName) * *arg.right) - (*arg.left * arg.right->diff(varName))) / (*arg.right ^ Expression<T>(T{2}));
        } else if constexpr (std::is_same_v<U, OperationPow>) {
            // Производная степени
            return (*arg.left ^ *arg.right) * ((arg.right->diff(varName) * (*arg.left).ln()) + (*arg.right * (arg.left->diff(varName) / *arg.left)));
        } else if constexpr (std::is_same_v<U, OperationSin>) {
            // Производная синуса
            return arg.arg->diff(varName) * arg.arg->cos();
        } else if constexpr (std::is_same_v<U, OperationCos>) {
            // Производная косинуса
            return Expression<T>(T{-1}) * arg.arg->diff(varName) * arg.arg->sin();
        } else if constexpr (std::is_same_v<U, OperationLn>) {
            // Производная логарифма
            return arg.arg->diff(varName) / *arg.arg;
        } else if constexpr (std::is_same_v<U, OperationExp>) {
            // Производная экспоненты
            return arg.arg->diff(varName) * (*arg.arg).exp();
        }
        return Expression<T>(T{0});
    }, expr_);
}

// Специализация для комплексных чисел
template <>
std::string Expression<std::complex<double>>::to_string() const {
    return std::visit([](auto&& arg) -> std::string {
        using U = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<U, Number>) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2);
            oss << "(" << arg.value.real() << " + " << arg.value.imag() << "i)";
            return oss.str();
        } else if constexpr (std::is_same_v<U, Variable>) {
            return arg.name;
        } else if constexpr (std::is_same_v<U, OperationAdd>) {
            return "(" + arg.left->to_string() + " + " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationSub>) {
            return "(" + arg.left->to_string() + " - " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationMul>) {
            return "(" + arg.left->to_string() + " * " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationDiv>) {
            return "(" + arg.left->to_string() + " / " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationPow>) {
            return "(" + arg.left->to_string() + " ^ " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationSin>) {
            return "sin(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationCos>) {
            return "cos(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationLn>) {
            return "ln(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationExp>) {
            return "exp(" + arg.arg->to_string() + ")";
        }
        return "";
    }, expr_);
}

// Реализация для остальных типов
template <typename T>
std::string Expression<T>::to_string() const {
    return std::visit([](auto&& arg) -> std::string {
        using U = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<U, Number>) {
            return std::to_string(arg.value);
        } else if constexpr (std::is_same_v<U, Variable>) {
            return arg.name;
        } else if constexpr (std::is_same_v<U, OperationAdd>) {
            return "(" + arg.left->to_string() + " + " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationSub>) {
            return "(" + arg.left->to_string() + " - " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationMul>) {
            return "(" + arg.left->to_string() + " * " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationDiv>) {
            return "(" + arg.left->to_string() + " / " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationPow>) {
            return "(" + arg.left->to_string() + " ^ " + arg.right->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationSin>) {
            return "sin(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationCos>) {
            return "cos(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationLn>) {
            return "ln(" + arg.arg->to_string() + ")";
        } else if constexpr (std::is_same_v<U, OperationExp>) {
            return "exp(" + arg.arg->to_string() + ")";
        }
        return "";
    }, expr_);
}



template <typename T>
Expression<T> Expression<T>::operator+(const Expression<T>& other) const {
    return Expression<T>(OperationAdd{
        std::make_shared<Expression<T>>(*this),
        std::make_shared<Expression<T>>(other)
    });
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression<T>& other) const {
    return Expression<T>(OperationSub{
        std::make_shared<Expression<T>>(*this),
        std::make_shared<Expression<T>>(other)
    });
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression<T>& other) const {
    return Expression<T>(OperationMul{
        std::make_shared<Expression<T>>(*this),
        std::make_shared<Expression<T>>(other)
    });
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression<T>& other) const {
    return Expression<T>(OperationDiv{
        std::make_shared<Expression<T>>(*this),
        std::make_shared<Expression<T>>(other)
    });
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression<T>& other) const {
    return Expression<T>(OperationPow{
        std::make_shared<Expression<T>>(*this),
        std::make_shared<Expression<T>>(other)
    });
}

template <typename T>
Expression<T> Expression<T>::sin() const {
    return Expression<T>(OperationSin{std::make_shared<Expression<T>>(*this)});
}

template <typename T>
Expression<T> Expression<T>::cos() const {
    return Expression<T>(OperationCos{std::make_shared<Expression<T>>(*this)});
}

template <typename T>
Expression<T> Expression<T>::ln() const {
    return Expression<T>(OperationLn{std::make_shared<Expression<T>>(*this)});
}

template <typename T>
Expression<T> Expression<T>::exp() const {
    return Expression<T>(OperationExp{std::make_shared<Expression<T>>(*this)});
}

template <typename T>
Expression<T> Expression<T>::substitute(const std::string& varName, const T& value) const {
    return std::visit([&](auto&& arg) -> Expression<T> {
        using U = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<U, Variable>) {
            if (arg.name == varName) {
                return Expression<T>(Number{value});
            } else {
                return Expression<T>(arg);
            }
        } else if constexpr (std::is_same_v<U, Number>) {
            return Expression<T>(arg);
        } else {
            return Expression<T>(arg);
        }
    }, expr_);
}

template class Expression<double>;

template class Expression<std::complex<double>>;