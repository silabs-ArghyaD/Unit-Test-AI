#include <gtest/gtest.h>
#include "../../../calculator.h"

TEST(CalculatorValidation, AddWorksForPositiveNumbers)
{
    EXPECT_EQ(add(2, 3), 5);
}

TEST(CalculatorValidation, AddWorksForNegativeNumbers)
{
    EXPECT_EQ(add(-2, -3), -5);
}

TEST(CalculatorValidation, SubtractWorksForPositiveNumbers)
{
    EXPECT_EQ(subtract(5, 3), 2);
}

TEST(CalculatorValidation, SubtractWorksForNegativeNumbers)
{
    EXPECT_EQ(subtract(-5, -3), -2);
}

TEST(CalculatorValidation, MultiplyWorksForPositiveNumbers)
{
    EXPECT_EQ(multiply(4, 3), 12);
}

TEST(CalculatorValidation, MultiplyWorksForZero)
{
    EXPECT_EQ(multiply(0, 5), 0);
}

TEST(CalculatorValidation, MultiplyWorksForNegativeNumbers)
{
    EXPECT_EQ(multiply(-2, 3), -6);
}

TEST(CalculatorValidation, DivideWorksForPositiveNumbers)
{
    EXPECT_EQ(divide(10, 2), 5);
}

TEST(CalculatorValidation, DivideByZeroReturnsZero)
{
    EXPECT_EQ(divide(10, 0), 0);
}

TEST(CalculatorValidation, DivideWorksForNegativeNumbers)
{
    EXPECT_EQ(divide(-10, 2), -5);
}

TEST(CalculatorValidation, IsEvenReturnsTrueForEvenNumbers)
{
    EXPECT_TRUE(is_even(4));
    EXPECT_TRUE(is_even(0));
}

TEST(CalculatorValidation, IsEvenReturnsFalseForOddNumbers)
{
    EXPECT_FALSE(is_even(3));
    EXPECT_FALSE(is_even(-1));
}

TEST(CalculatorValidation, MaxReturnsGreaterValue)
{
    EXPECT_EQ(max(5, 3), 5);
    EXPECT_EQ(max(-2, -5), -2);
}

TEST(CalculatorValidation, MaxReturnsEqualValueIfSame)
{
    EXPECT_EQ(max(7, 7), 7);
}

TEST(CalculatorValidation, MinReturnsSmallerValue)
{
    EXPECT_EQ(min(2, 5), 2);
    EXPECT_EQ(min(-2, -5), -5);
}

TEST(CalculatorValidation, MinReturnsEqualValueIfSame)
{
    EXPECT_EQ(min(4, 4), 4);
}