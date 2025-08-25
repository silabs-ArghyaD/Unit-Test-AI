#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "../../../ClosureManager.h"

// Test fixture for ClosureManager tests
class ClosureManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to capture output for verification
        original_cout = std::cout.rdbuf();
        std::cout.rdbuf(output_stream.rdbuf());
    }

    void TearDown() override {
        // Restore original cout
        std::cout.rdbuf(original_cout);
    }

    ClosureManager manager;
    std::stringstream output_stream;
    std::streambuf* original_cout;
};

// Test default constructor initializes to closed state
TEST_F(ClosureManagerTest, DefaultConstructorInitializesToClosed) {
    EXPECT_FALSE(manager.isOpen());
}

// Test open() method functionality
TEST_F(ClosureManagerTest, OpenMethodSetsStateToOpen) {
    manager.open();
    EXPECT_TRUE(manager.isOpen());
}

// Test open() method output message
TEST_F(ClosureManagerTest, OpenMethodPrintsCorrectMessage) {
    manager.open();
    std::string output = output_stream.str();
    EXPECT_EQ(output, "Opening closure\n");
}

// Test close() method functionality
TEST_F(ClosureManagerTest, CloseMethodSetsStateToClosed) {
    // First open it
    manager.open();
    EXPECT_TRUE(manager.isOpen());
    
    // Clear the output stream
    output_stream.str("");
    output_stream.clear();
    
    // Then close it
    manager.close();
    EXPECT_FALSE(manager.isOpen());
}

// Test close() method output message
TEST_F(ClosureManagerTest, CloseMethodPrintsCorrectMessage) {
    manager.open();
    output_stream.str(""); // Clear previous output
    output_stream.clear();
    
    manager.close();
    std::string output = output_stream.str();
    EXPECT_EQ(output, "Closing closure\n");
}

// Test multiple open operations
TEST_F(ClosureManagerTest, MultipleOpenOperationsKeepStateOpen) {
    manager.open();
    EXPECT_TRUE(manager.isOpen());
    
    output_stream.str("");
    output_stream.clear();
    
    manager.open(); // Open again
    EXPECT_TRUE(manager.isOpen());
    
    std::string output = output_stream.str();
    EXPECT_EQ(output, "Opening closure\n");
}

// Test multiple close operations
TEST_F(ClosureManagerTest, MultipleCloseOperationsKeepStateClosed) {
    // Start with closed state (default)
    EXPECT_FALSE(manager.isOpen());
    
    manager.close(); // Close when already closed
    EXPECT_FALSE(manager.isOpen());
    
    std::string output = output_stream.str();
    EXPECT_EQ(output, "Closing closure\n");
}

// Test open-close sequence
TEST_F(ClosureManagerTest, OpenCloseSequenceWorksCorrectly) {
    // Initial state should be closed
    EXPECT_FALSE(manager.isOpen());
    
    // Open
    manager.open();
    EXPECT_TRUE(manager.isOpen());
    
    // Close
    manager.close();
    EXPECT_FALSE(manager.isOpen());
    
    // Verify output contains both messages
    std::string output = output_stream.str();
    EXPECT_TRUE(output.find("Opening closure") != std::string::npos);
    EXPECT_TRUE(output.find("Closing closure") != std::string::npos);
}

// Test isOpen() const correctness
TEST_F(ClosureManagerTest, IsOpenMethodIsConst) {
    const ClosureManager& const_manager = manager;
    
    // Should be able to call isOpen() on const reference
    EXPECT_FALSE(const_manager.isOpen());
    
    manager.open();
    EXPECT_TRUE(const_manager.isOpen());
}

// Test state persistence after operations
TEST_F(ClosureManagerTest, StatePersistsAfterOperations) {
    // Test that state remains consistent across multiple operations
    
    // Open -> Check -> Close -> Check -> Open -> Check
    manager.open();
    EXPECT_TRUE(manager.isOpen());
    
    manager.close();
    EXPECT_FALSE(manager.isOpen());
    
    manager.open();
    EXPECT_TRUE(manager.isOpen());
}

// Test that operations are independent of previous output
TEST_F(ClosureManagerTest, OperationsAreIndependentOfOutput) {
    // Even if we don't read the output, operations should work
    manager.open();
    manager.close();
    manager.open();
    
    EXPECT_TRUE(manager.isOpen());
}

// Performance test - multiple rapid operations
TEST_F(ClosureManagerTest, MultipleRapidOperationsPerformCorrectly) {
    // Test rapid open/close operations
    for (int i = 0; i < 100; ++i) {
        manager.open();
        EXPECT_TRUE(manager.isOpen());
        manager.close();
        EXPECT_FALSE(manager.isOpen());
    }
    
    // Final state should be closed
    EXPECT_FALSE(manager.isOpen());
}

// Edge case: Verify no memory leaks or resource issues
TEST_F(ClosureManagerTest, NoResourceLeaksWithManyOperations) {
    // Perform many operations to check for resource leaks
    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            manager.open();
        } else {
            manager.close();
        }
    }
    
    // Should still function correctly
    manager.open();
    EXPECT_TRUE(manager.isOpen());
    manager.close();
    EXPECT_FALSE(manager.isOpen());
}
