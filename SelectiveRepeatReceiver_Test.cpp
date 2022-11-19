#include "SelectiveRepeatReceiver.h"
#include "SelectiveRepeatSender.h"
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

TEST_F(ReceiverTest, receiveTest) {
  auto srs = new SelectiveRepeatSender;
  for(int i = 0; i < 8; i++) {
    srs->prepareNext();
    srp->receive(srs->send());
    EXPECT_FALSE(srp->getDeliverableSeq() == 9);
    srp->deliver();
  }
  for(int i = 0; i < 8; i++) {
    srp->acknowledge();
  }
  srp->receive(srs->send());
  srs->prepareNext();
  srs->prepareNext();
  srp->receive(8);
  srp->receive(9);
  srp->receive(10);
  srp->deliver();
  srp->deliver();
  EXPECT_EQ(srp->getDeliverableSeq(), 10);
  srp->receive(55);
  EXPECT_FALSE(srp->getDeliverableSeq() == 55);
  srp->receive(255);
  EXPECT_FALSE(srp->getDeliverableSeq() == 255);
  srp->receive(999);
  EXPECT_FALSE(srp->getDeliverableSeq() == 999);
  delete srs;
}

TEST_F(ReceiverTest, getDeliverableSeqInit) {
  EXPECT_EQ(srp->getDeliverableSeq(), -1);
  auto srs = new SelectiveRepeatSender;
  for(int i = 0; i < 8; i++) {
    srs->prepareNext();
    srp->receive(srs->send());
    EXPECT_EQ(srp->getDeliverableSeq(), i);
    srp->deliver();
  }
  delete srs;
}

TEST_F(ReceiverTest, deliverTest) {
  EXPECT_EQ(srp->deliver(), -1);
  srp->receive(1); 
  EXPECT_EQ(srp->deliver(), -1);
  srp->receive(0);
  EXPECT_EQ(srp->deliver(), 0);
  srp->receive(3);
  EXPECT_EQ(srp->deliver(), 1);
  srp->receive(2);
  EXPECT_EQ(srp->deliver(), 2);
  srp->receive(6); // seeing if 3 is received (selective repeat)
  EXPECT_EQ(srp->deliver(), 3);
  EXPECT_EQ(srp->deliver(), -1);
  srp->receive(3);
  EXPECT_EQ(srp->deliver(), -1);
  EXPECT_NE(srp->deliver(), 3);
}

TEST_F(ReceiverTest, ackTest) {
  EXPECT_EQ(srp->acknowledge().sequence, -1);
  srp->receive(1); // stored
  EXPECT_EQ(srp->acknowledge().sequence, -1);
  srp->receive(0); // ack(1) - storing 0 and storing 1!
  EXPECT_EQ(srp->acknowledge().sequence, 1);
  srp->receive(3);
  EXPECT_EQ(srp->acknowledge().sequence, 1); // should be 1
  srp->receive(2);
  EXPECT_EQ(srp->acknowledge().sequence, 3);
}

TEST_F(ReceiverTest, getAckTest) {
  EXPECT_EQ(srp->getAcknowledgable(), -1);
  srp->receive(1); // stored
  EXPECT_EQ(srp->getAcknowledgable(), -1);
  srp->receive(0); // ack(1) - storing 0 and storing 1!
  EXPECT_EQ(srp->getAcknowledgable(), 1);
  srp->receive(3);
  EXPECT_EQ(srp->getAcknowledgable(), 1);
  srp->receive(2);
  EXPECT_EQ(srp->getAcknowledgable(), 3);
}