/*#include "Expression.hpp"
#include <iostream>
#include <complex>*/
#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*int main() {
    Expression<double> expr1 = (make_value(100.0) + make_variable<double>("x") * make_value(2.0)).sin();
    std::map<std::string, double> context1 = {{"x", 10.0}};
    Expression<double> derivative1 = expr1.diff("x");

    std::cout << "Value (double): " << expr1.eval(context1) << std::endl; // 120.0
    std::cout << "String: " << derivative1.to_string() << std::endl; // (100.000000 + (x * 2.000000))
    using Complex = std::complex<double>;
    Expression<Complex> expr2 = make_variable<Complex>("x") + make_variable<Complex>("y");
    std::map<std::string, Complex> context2 = {
        {"x", Complex(1.0, 2.0)}, // x = 1 + 2i
        {"y", Complex(3.0, 4.0)}  // y = 3 + 4i
    };
    std::cout << "Value (complex): " << expr2.eval(context2) << std::endl; // (4 + 6i)

    return 0;
}*/