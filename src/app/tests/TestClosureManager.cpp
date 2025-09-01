#include <gtest/gtest.h>
#include "../../../ClosureManager.h"

// Test that ClosureManager starts in closed state
TEST(ClosureManagerTest, StartsInClosedState) {
    ClosureManager manager;
    EXPECT_FALSE(manager.isOpen());
}

// Test that open() works
TEST(ClosureManagerTest, OpenSetsStateToOpen) {
    ClosureManager manager;
    manager.open();
    EXPECT_TRUE(manager.isOpen());
}

// Test that close() works
TEST(ClosureManagerTest, CloseSetsStateToClosed) {
    ClosureManager manager;
    manager.open();
    manager.close();
    EXPECT_FALSE(manager.isOpen());
}
