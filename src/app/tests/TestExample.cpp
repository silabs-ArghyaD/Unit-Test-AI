#include <pw_unit_test/framework.h>
#include "../../../test_sample.c"

class TestSampleValidation : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(TestSampleValidation, AddFunctionReturnsCorrectSum) {
    EXPECT_EQ(add(5, 3), 8);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(10, -5), 5);
}

TEST_F(TestSampleValidation, SubtractFunctionReturnsCorrectDifference) {
    EXPECT_EQ(subtract(10, 4), 6);
    EXPECT_EQ(subtract(0, 0), 0);
    EXPECT_EQ(subtract(5, 10), -5);
    EXPECT_EQ(subtract(-3, -8), 5);
}

TEST_F(TestSampleValidation, MultiplyFunctionReturnsCorrectProduct) {
    EXPECT_EQ(multiply(4, 5), 20);
    EXPECT_EQ(multiply(0, 10), 0);
    EXPECT_EQ(multiply(-2, 3), -6);
    EXPECT_EQ(multiply(-4, -5), 20);
}

TEST_F(TestSampleValidation, FactorialFunctionReturnsCorrectValue) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(5), 120);
    EXPECT_EQ(factorial(4), 24);
    EXPECT_EQ(factorial(6), 720);
}

TEST_F(TestSampleValidation, FactorialEdgeCases) {
    // Test edge cases
    EXPECT_EQ(factorial(-1), 1);  // Should return 1 for negative numbers based on implementation
    EXPECT_EQ(factorial(0), 1);   // 0! = 1
}
