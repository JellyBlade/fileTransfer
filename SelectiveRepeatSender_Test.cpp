#include "SelectiveRepeatSender.h"
#include "gtest/gtest.h"
#include <iostream>

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

TEST_F(SenderTest, sendTest) {
  EXPECT_EQ(srs->send(), -1);
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_EQ(srs->send(), 0);
  srs->prepareNext();
  srs->prepareNext();
  std::vector<int> seqs = srs->sendAll();
  EXPECT_EQ(seqs.size(), 2);
  EXPECT_EQ(srs->send(), -1);
  // wip, please finish me :)
}
