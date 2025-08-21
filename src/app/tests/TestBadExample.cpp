#include <pw_unit_test/framework.h>
// Missing the component .cpp file include - MISTAKE 1

class TestBadExample : public ::testing::Test {
protected:
    // Using undefined class since no include
    SomeManager manager;
};

// Wrong naming - doesn't start with TEST - MISTAKE 2
void CheckInitialState() {
    // This should use EXPECT_ but using regular assert - not a validation error but bad practice
    assert(false);
}

// Wrong test suite naming - MISTAKE 3
TEST_F(BadNaming, CanDoSomething) {
    EXPECT_TRUE(true);
}

// This one is correct for comparison
TEST_F(TestBadExample, CorrectTestExample) {
    EXPECT_FALSE(false);
}

// Another wrong function name - MISTAKE 4
int validateSomething() {
    return 0;
}

// Wrong suite naming again - MISTAKE 5
TEST(IncorrectSuite, AnotherTest) {
    EXPECT_EQ(1, 1);
}
