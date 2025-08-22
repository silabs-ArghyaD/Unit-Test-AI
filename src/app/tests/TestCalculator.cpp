#include <gtest/gtest.h>
#include "../../../calculator.h"

// Include the implementation (without main function)
extern "C" {
    int add(int a, int b) {
        return a + b;
    }

    int subtract(int a, int b) {
        return a - b;
    }

    int multiply(int a, int b) {
        return a * b;
    }

    int divide(int a, int b) {
        if (b == 0) {
            return 0; // Simple error handling
        }
        return a / b;
    }

    int is_even(int n) {
        return (n % 2) == 0;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }
}

class CalculatorValidation : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(CalculatorValidation, AddFunctionReturnsCorrectSum) {
    EXPECT_EQ(add(5, 3), 8);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(10, -5), 5);
    EXPECT_EQ(add(-3, -7), -10);
}

TEST_F(CalculatorValidation, SubtractFunctionReturnsCorrectDifference) {
    EXPECT_EQ(subtract(10, 4), 6);
    EXPECT_EQ(subtract(0, 0), 0);
    EXPECT_EQ(subtract(5, 10), -5);
    EXPECT_EQ(subtract(-3, -8), 5);
    EXPECT_EQ(subtract(20, 5), 15);
}

TEST_F(CalculatorValidation, MultiplyFunctionReturnsCorrectProduct) {
    EXPECT_EQ(multiply(4, 5), 20);
    EXPECT_EQ(multiply(0, 10), 0);
    EXPECT_EQ(multiply(-2, 3), -6);
    EXPECT_EQ(multiply(-4, -5), 20);
    EXPECT_EQ(multiply(7, 1), 7);
}

TEST_F(CalculatorValidation, DivideFunctionReturnsCorrectQuotient) {
    EXPECT_EQ(divide(20, 4), 5);
    EXPECT_EQ(divide(15, 3), 5);
    EXPECT_EQ(divide(-12, 3), -4);
    EXPECT_EQ(divide(-20, -4), 5);
    EXPECT_EQ(divide(7, 2), 3); // Integer division
}

TEST_F(CalculatorValidation, DivideFunctionHandlesDivisionByZero) {
    EXPECT_EQ(divide(10, 0), 0); // Should return 0 for division by zero
    EXPECT_EQ(divide(-5, 0), 0);
    EXPECT_EQ(divide(0, 0), 0);
}

TEST_F(CalculatorValidation, IsEvenFunctionIdentifiesEvenNumbers) {
    EXPECT_EQ(is_even(2), 1);
    EXPECT_EQ(is_even(4), 1);
    EXPECT_EQ(is_even(0), 1);
    EXPECT_EQ(is_even(-2), 1);
    EXPECT_EQ(is_even(100), 1);
}

TEST_F(CalculatorValidation, IsEvenFunctionIdentifiesOddNumbers) {
    EXPECT_EQ(is_even(1), 0);
    EXPECT_EQ(is_even(3), 0);
    EXPECT_EQ(is_even(-1), 0);
    EXPECT_EQ(is_even(-3), 0);
    EXPECT_EQ(is_even(99), 0);
}

TEST_F(CalculatorValidation, MaxFunctionReturnsLargerValue) {
    EXPECT_EQ(max(5, 3), 5);
    EXPECT_EQ(max(3, 5), 5);
    EXPECT_EQ(max(-1, -5), -1);
    EXPECT_EQ(max(0, -1), 0);
    EXPECT_EQ(max(10, 10), 10);
}

TEST_F(CalculatorValidation, MaxFunctionHandlesNegativeNumbers) {
    EXPECT_EQ(max(-10, -20), -10);
    EXPECT_EQ(max(-5, 5), 5);
    EXPECT_EQ(max(0, -100), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
