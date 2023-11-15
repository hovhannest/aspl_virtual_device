#include <gtest/gtest.h>
#include "foo.h"

TEST(FooTest, Returns42) {
    EXPECT_EQ(foo(), 42);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
