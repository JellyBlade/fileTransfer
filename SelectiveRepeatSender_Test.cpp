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
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  EXPECT_EQ(srs->send(), 3); 
  EXPECT_EQ(srs->send(), 4);
  EXPECT_EQ(srs->send(), 5);
  srs->prepareNext();
  EXPECT_FALSE(srs->prepareNext());
}

TEST_F(SenderTest, getSentCountTest) {
  for (int i = 1; i < 8; i++) {
  srs->prepareNext();
  srs->send();
  EXPECT_EQ(srs->getSentCount(), i);
  }
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {0, {}}));
  EXPECT_EQ(srs->getSentCount(), 6);
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {4, {}}));
  EXPECT_EQ(srs->getSentCount(), 3);
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {5, {}}));
  EXPECT_EQ(srs->getSentCount(), 2);
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {7, {}}));
  EXPECT_EQ(srs->getSentCount(), 0);
}

TEST_F(SenderTest, prepareNextTest) {
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_TRUE(srs->prepareNext());
  EXPECT_FALSE(srs->prepareNext()); // can't prepare next as window is full!
}

TEST_F(SenderTest, ackSenderTest) {
  EXPECT_FALSE(srs->acknowledge((ReceiverAck) {-1, {}}));
  srs->prepareNext();
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {0, {}}));
  srs->prepareNext();
  EXPECT_FALSE(srs->acknowledge((ReceiverAck) {0, {}}));
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {1, {}}));
  srs->prepareNext();
  EXPECT_FALSE(srs->acknowledge((ReceiverAck) {1, {}}));
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {2, {}}));
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {7, {}}));
  EXPECT_FALSE(srs->acknowledge((ReceiverAck) {9, {}}));
  srs->prepareNext();
  srs->prepareNext();
  srs->prepareNext();
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {8, {10}}));
  EXPECT_TRUE(srs->acknowledge((ReceiverAck) {10, {}}));
}