#include <gtest/gtest.h>
#include "Expression.hpp"

class ExpressionTest : public ::testing::Test {
protected:
    void SetUp() override {}
};
// Тест для конструктора и метода toString
TEST_F(ExpressionTest, ConstructorAndToStringAndDiff) {
    Expression<double> expr1 = (make_value(100.0) + make_variable<double>("x") * make_value(2.0)).sin();
    Expression<double> derivative1 = expr1.diff("x");
    EXPECT_EQ(derivative1.to_string(), "((0.000000 + ((1.000000 * 2.000000) + (x * 0.000000))) * cos((100.000000 + (x * 2.000000))))");
}
TEST_F(ExpressionTest, ConstructorAndToStringAdDiff) {
    Expression<double> expr1 = (make_variable<double>("x") ^ make_variable<double>("y") );
    Expression<double> derivative1 = expr1.diff("x");
    EXPECT_EQ(derivative1.to_string(), "(1.000000 / x)");
}


// Тест для создания выражения из переменной
TEST_F(ExpressionTest, CreateFromVariable) {
    Expression<double> expr = make_variable<double>("x");
    std::map<std::string, double> context = {{"x", 10.0}};
    EXPECT_EQ(expr.eval(context), 10.0);
}

// Тест для сложения
TEST_F(ExpressionTest, Addition) {
    Expression<double> expr = make_value(5.0) + make_value(3.0);
    EXPECT_EQ(expr.eval({}), 8.0);
}
// Тест для синуса
TEST_F(ExpressionTest, SinFunction) {
    Expression<double> expr = make_value(0.0).sin();
    EXPECT_NEAR(expr.eval({{}}), 0.0, 1e-6);
}

// Тест для экспоненты
TEST_F(ExpressionTest, ExpFunction) {
    Expression<double> expr = make_value(1.0).exp();
    EXPECT_NEAR(expr.eval({{}}), std::exp(1.0), 1e-6);
}





