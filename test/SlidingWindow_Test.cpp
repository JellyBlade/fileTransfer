#include "SlidingWindow.h"
#include "gtest/gtest.h"

#include <iostream>

/**
 * @brief Tests for SlidingWindow
 * 
 */
class SlidingWindowTest : public testing::Test {
 protected:
  SlidingWindow* sw;

  void SetUp() override {
    sw = new SlidingWindow();
  }

  void TearDown() override {
    delete sw;
  }
};

TEST_F(SlidingWindowTest, canAdvanceTest) {
  EXPECT_TRUE(sw->canAdvance());
  for (int i = 0; i < 8; i++) { sw->advance(); }
  EXPECT_FALSE(sw->canAdvance());
  sw->setWindowSize(9);
  EXPECT_TRUE(sw->canAdvance());
  sw->advance();
  EXPECT_FALSE(sw->canAdvance());
  auto s = sw->getWindowSequences()->front();
  sw->getWindowSequences()->pop_front();
  EXPECT_TRUE(sw->canAdvance());
  delete s;
}

TEST_F(SlidingWindowTest, advanceTest) {
  EXPECT_EQ(sw->advance(), 0);
  for (int i = 0; i < 7; i++) { sw->advance(); }
  EXPECT_EQ(sw->advance(), -1);
  sw->setWindowSize(20);
  EXPECT_EQ(sw->advance(), 8);
  EXPECT_EQ(sw->advance(), 9);
  EXPECT_EQ(sw->advance(), 10);
  EXPECT_TRUE(sw->canAdvance());
  EXPECT_EQ(sw->advance(), 11);
  sw->setWindowSize(11);
  EXPECT_FALSE(sw->canAdvance());
  auto ws = sw->getWindowSequences();
  auto s1 = ws->front();
  ws->pop_front();
  auto s2 = ws->front();
  ws->pop_front();
  EXPECT_TRUE(sw->canAdvance());
  delete s1;
  delete s2;
}

TEST_F(SlidingWindowTest, windowSizeTest) {
  EXPECT_EQ(sw->getWindowSize(), 8);
  sw->setWindowSize(12);
  EXPECT_EQ(sw->getWindowSize(), 12);
  sw->setWindowSize(256);
  EXPECT_EQ(sw->getWindowSize(), 255);
  sw->setBitSize(16);
  sw->setWindowSize(65536);
  EXPECT_EQ(sw->getWindowSize(), 65535);
  sw->setWindowSize(34853123);
  EXPECT_EQ(sw->getWindowSize(), 65535);
}

TEST_F(SlidingWindowTest, bitSizeTest) {
  EXPECT_EQ(sw->getBitSize(), 8);
  sw->setBitSize(10);
  EXPECT_EQ(sw->getBitSize(), 10);
  sw->setBitSize(64);
  EXPECT_EQ(sw->getBitSize(), 64);
}

TEST_F(SlidingWindowTest, seqCountTest) {
  EXPECT_EQ(sw->seqCount(), 0);
  sw->advance();
  EXPECT_EQ(sw->seqCount(), 1);
  auto s = sw->getWindowSequences()->front();
  sw->getWindowSequences()->pop_front();
  EXPECT_EQ(sw->seqCount(), 0);
  for (int i = 0; i < 8; i++) { sw->advance(); }
  EXPECT_EQ(sw->seqCount(), 8);
  EXPECT_EQ(sw->advance(), -1);
  EXPECT_EQ(sw->seqCount(), 8);
  delete s;
}

TEST_F(SlidingWindowTest, sequenceNumberTest) {
  EXPECT_EQ(sw->getCurrentSequence(), -1);
  sw->advance();
  EXPECT_EQ(sw->getCurrentSequence(), 0);
  for (int i = 0; i < 7; i++) { sw->advance(); }
  EXPECT_EQ(sw->getCurrentSequence(), 7);
  sw->advance();
  EXPECT_EQ(sw->getCurrentSequence(), 7);
  for (int i = 0; i < 248; i++) {
    auto s = sw->getWindowSequences()->front();
    sw->getWindowSequences()->pop_front();
    delete s;
    sw->advance();
  }
  EXPECT_EQ(sw->getCurrentSequence(), 255);
  auto s = sw->getWindowSequences()->front();
  sw->getWindowSequences()->pop_front();
  delete s;
  sw->advance();
  EXPECT_EQ(sw->getCurrentSequence(), 0);
}
