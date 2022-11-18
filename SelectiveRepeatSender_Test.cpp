#include "SelectiveRepeatSender.h"
#include "gtest/gtest.h"

class SenderTest : public testing::Test {
 protected:
  SelectiveRepeatSender* srs;
};

TEST_F(SenderTest, testTest) {
  EXPECT_EQ(1, 1);
}
