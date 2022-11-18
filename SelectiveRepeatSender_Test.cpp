#include "SelectiveRepeatSender.h"
#include "gtest/gtest.h"

class SenderTest : public testing::Test {
 protected:
  SelectiveRepeatSender* srs;

  void SetUp() override {
    srs = new SelectiveRepeatSender();
  }

  void TearDown() override {
    delete srs;
  }
};

TEST_F(SenderTest, testTest) {
  EXPECT_EQ(1, 1);
}
