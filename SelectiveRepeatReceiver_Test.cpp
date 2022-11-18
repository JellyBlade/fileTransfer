#include "SelectiveRepeatReceiver.h"
#include "gtest/gtest.h"

class ReceiverTest : public testing::Test {
 protected:
  SelectiveRepeatReceiver* srp;

  void SetUp() override {
    srp = new SelectiveRepeatReceiver();
  }

  void TearDown() override {
    delete srp;
  }
};

TEST_F(ReceiverTest, testTest) {
  EXPECT_EQ(1, 1);
}
