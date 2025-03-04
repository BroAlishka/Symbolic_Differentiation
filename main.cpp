#include "Expression.hpp"
#include <iostream>

int main() {
    Expression<double> x("x");
    Expression<double> five(5.0);
    Expression<double> expr = x + five;

    std::cout << "Expression: " << expr.toString() << std::endl;



    return 0;
}