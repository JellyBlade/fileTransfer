#include "SlidingWindow.h"
#include "gtest/gtest.h"

class SlidingWindowTest : public testing::Test {
 protected:
  SlidingWindow* sw;
};

TEST_F(SlidingWindowTest, testTest) {
  EXPECT_EQ(1, 1);
}

TEST_F(SlidingWindowTest, brokenTest) {
  EXPECT_EQ(1, 0);
}



