#include <lib/core/StringBuilderAdapters.h>
#include <pw_unit_test/framework.h>
#include <app/data-model/Nullable.h>

// Include your component's header file
#include <app/clusters/calculator/calculator.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::DataModel;
using namespace chip::app::Clusters::Calculator;

// Test Suite: CalculatorValidation

extern "C" {
#include "calculator.h"
}

TEST(CalculatorValidation, AddWorksWithPositiveNumbers)
{
    EXPECT_EQ(add(2, 3), 5);
}

TEST(CalculatorValidation, AddWorksWithNegativeNumbers)
{
    EXPECT_EQ(add(-2, -3), -5);
}

TEST(CalculatorValidation, AddWorksWithZero)
{
    EXPECT_EQ(add(0, 5), 5);
    EXPECT_EQ(add(5, 0), 5);
}

TEST(CalculatorValidation, SubtractWorksWithPositiveNumbers)
{
    EXPECT_EQ(subtract(5, 3), 2);
}

TEST(CalculatorValidation, SubtractWorksWithNegativeNumbers)
{
    EXPECT_EQ(subtract(-5, -3), -2);
}

TEST(CalculatorValidation, SubtractWorksWithZero)
{
    EXPECT_EQ(subtract(0, 5), -5);
    EXPECT_EQ(subtract(5, 0), 5);
}

TEST(CalculatorValidation, MultiplyWorksWithPositiveNumbers)
{
    EXPECT_EQ(multiply(4, 3), 12);
}

TEST(CalculatorValidation, MultiplyWorksWithNegativeNumbers)
{
    EXPECT_EQ(multiply(-4, 3), -12);
    EXPECT_EQ(multiply(4, -3), -12);
    EXPECT_EQ(multiply(-4, -3), 12);
}

TEST(CalculatorValidation, MultiplyWorksWithZero)
{
    EXPECT_EQ(multiply(0, 5), 0);
    EXPECT_EQ(multiply(5, 0), 0);
}

TEST(CalculatorValidation, DivideWorksWithPositiveNumbers)
{
    EXPECT_EQ(divide(10, 2), 5);
}

TEST(CalculatorValidation, DivideWorksWithNegativeNumbers)
{
    EXPECT_EQ(divide(-10, 2), -5);
    EXPECT_EQ(divide(10, -2), -5);
    EXPECT_EQ(divide(-10, -2), 5);
}

TEST(CalculatorValidation, DivideByZeroReturnsZero)
{
    EXPECT_EQ(divide(10, 0), 0);
    EXPECT_EQ(divide(0, 0), 0);
}

TEST(CalculatorValidation, DivideZeroNumerator)
{
    EXPECT_EQ(divide(0, 5), 0);
}

TEST(CalculatorValidation, IsEvenDetectsEvenNumbers)
{
    EXPECT_TRUE(is_even(2));
    EXPECT_TRUE(is_even(0));
    EXPECT_TRUE(is_even(-4));
}

TEST(CalculatorValidation, IsEvenDetectsOddNumbers)
{
    EXPECT_FALSE(is_even(3));
    EXPECT_FALSE(is_even(-1));
}

TEST(CalculatorValidation, MaxReturnsCorrectValue)
{
    EXPECT_EQ(max(5, 3), 5);
    EXPECT_EQ(max(3, 5), 5);
    EXPECT_EQ(max(-1, -5), -1);
    EXPECT_EQ(max(7, 7), 7);
}

TEST(CalculatorValidation, MinReturnsCorrectValue)
{
    EXPECT_EQ(min(5, 3), 3);
    EXPECT_EQ(min(3, 5), 3);
    EXPECT_EQ(min(-1, -5), -5);
    EXPECT_EQ(min(7, 7), 7);
}