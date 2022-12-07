// headerManager GTests
// Tyler Hippard <tyler.hippard@uleth.ca>

#include "HeaderManager.h"
#include "gtest/gtest.h"

class HeaderManagerTest : public testing::Test {
 protected:
  HeaderManager* hm;

  // I didn't know you could do this in GTest. I love this.
  void SetUp() override {
    hm = new HeaderManager;
  }

  void TearDown() override {
    delete hm;
  }
};

TEST_F(HeaderManagerTest, testType) {

  // empty on creation
  ASSERT_TRUE(hm->getType() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);

  // valid types
  hm->setType(1);
  ASSERT_TRUE(hm->getType() == 1);
  ASSERT_TRUE(hm->getHeader()[0] == 0x40);
  hm->setType(3);
  ASSERT_TRUE(hm->getType() == 3);
  ASSERT_TRUE(hm->getHeader()[0] == 0xc0);

  // invalid types
  hm->setType(4);
  ASSERT_TRUE(hm->getType() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
  hm->setType(500);
  ASSERT_TRUE(hm->getType() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
  hm->setType(-1);
  ASSERT_TRUE(hm->getType() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
}

TEST_F(HeaderManagerTest, testTR) {

  // empty on creation
  ASSERT_TRUE(hm->getTR() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);

  // valid TR
  hm->setTR(1);
  ASSERT_TRUE(hm->getTR() == 1);
  ASSERT_TRUE(hm->getHeader()[0] == 0x20);

  // invalid TRs
  hm->setTR(2);
  ASSERT_TRUE(hm->getTR() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
  hm->setTR(-1);
  ASSERT_TRUE(hm->getTR() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
}

TEST_F(HeaderManagerTest, testWindow) {

  // empty on creation
  ASSERT_TRUE(hm->getWindow() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);

  // valid windows
  hm->setWindow(1);
  ASSERT_TRUE(hm->getWindow() == 1);
  ASSERT_TRUE(hm->getHeader()[0] == 0x01);
  hm->setWindow(31);
  ASSERT_TRUE(hm->getWindow() == 31);
  ASSERT_TRUE(hm->getHeader()[0] == 0x1f);
  hm->setWindow(15);
  ASSERT_TRUE(hm->getWindow() == 15);
  ASSERT_TRUE(hm->getHeader()[0] == 0x0f);

  // invalid windows
  hm->setWindow(-1);
  ASSERT_TRUE(hm->getWindow() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
  hm->setWindow(32);
  ASSERT_TRUE(hm->getWindow() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
  hm->setWindow(5000);
  ASSERT_TRUE(hm->getWindow() == 0);
  ASSERT_TRUE(hm->getHeader()[0] == 0x00);
}

TEST_F(HeaderManagerTest, testSeqNum) {

  // empty on creation
  ASSERT_TRUE(hm->getSeqNum() == 0);
  ASSERT_TRUE(hm->getHeader()[1] == 0x00);

  // valid seqNums
  hm->setSeqNum(1);
  ASSERT_TRUE(hm->getSeqNum() == 1);
  ASSERT_TRUE(hm->getHeader()[1] == 0x01);
  hm->setSeqNum(255);
  ASSERT_TRUE(hm->getSeqNum() == 255);
  ASSERT_TRUE(hm->getHeader()[1] == 0xff);
  hm->setSeqNum(128);
  ASSERT_TRUE(hm->getSeqNum() == 128);
  ASSERT_TRUE(hm->getHeader()[1] == 0x80);

  // invalid seqNums
  hm->setSeqNum(256);
  ASSERT_TRUE(hm->getSeqNum() == 0);
  ASSERT_TRUE(hm->getHeader()[1] == 0x00);
  hm->setSeqNum(-1);
  ASSERT_TRUE(hm->getSeqNum() == 0);
  ASSERT_TRUE(hm->getHeader()[1] == 0x00);
  hm->setSeqNum(5000);
  ASSERT_TRUE(hm->getSeqNum() == 0);
  ASSERT_TRUE(hm->getHeader()[1] == 0x00);
}

TEST_F(HeaderManagerTest, testLength) {

  // empty on creation
  ASSERT_TRUE(hm->getLength() == 0);
  ASSERT_TRUE(hm->getHeader()[2] == 0x00 && hm->getHeader()[3] == 0x00);

  // valid lengths
  hm->setLength(1);
  ASSERT_TRUE(hm->getLength() == 1);
  ASSERT_TRUE(hm->getHeader()[2] == 0x00 && hm->getHeader()[3] == 0x01);
  hm->setLength(65535);
  ASSERT_TRUE(hm->getLength() == 65535);
  ASSERT_TRUE(hm->getHeader()[2] == 0xff && hm->getHeader()[3] == 0xff);
  hm->setLength(32768);
  ASSERT_TRUE(hm->getLength() == 32768);
  ASSERT_TRUE(hm->getHeader()[2] == 0x80 && hm->getHeader()[3] == 0x00);
  hm->setLength(10000);
  ASSERT_TRUE(hm->getLength() == 10000);
  ASSERT_TRUE(hm->getHeader()[2] == 0x27 && hm->getHeader()[3] == 0x10);

  // invalid lengths
  hm->setLength(-1);
  ASSERT_TRUE(hm->getLength() == 0);
  ASSERT_TRUE(hm->getHeader()[2] == 0x00 && hm->getHeader()[3] == 0x00);
  hm->setLength(65536);
  ASSERT_TRUE(hm->getLength() == 0);
  ASSERT_TRUE(hm->getHeader()[2] == 0x00 && hm->getHeader()[3] == 0x00);
  hm->setLength(5083040);
  ASSERT_TRUE(hm->getLength() == 0);
  ASSERT_TRUE(hm->getHeader()[2] == 0x00 && hm->getHeader()[3] == 0x00);
}

TEST_F(HeaderManagerTest, testCombined) {
  // entire header is empty on creation
  ASSERT_TRUE(hm->getHeader()[0] == 0x00
           && hm->getHeader()[1] == 0x00
	   && hm->getHeader()[2] == 0x00
	   && hm->getHeader()[3] == 0x00);
  // setting first four bytes
  hm->setType(2);
  hm->setTR(1);
  hm->setWindow(26);
  ASSERT_TRUE(hm->getWindow() == 26);
  ASSERT_TRUE(hm->getTR() == 1);
  ASSERT_TRUE(hm->getType() == 2);
  ASSERT_TRUE(hm->getHeader()[0] == 0xba);

  // setting second byte
  hm->setSeqNum(240);
  ASSERT_TRUE(hm->getHeader()[1] == 0xf0);

  // setting last two bytes
  hm->setLength(5000);
  ASSERT_TRUE(hm->getHeader()[2] == 0x13 && hm->getHeader()[3] == 0x88);

  // combining all four bytes together by shifting to the left 8 and ORing a few times.
  ASSERT_TRUE(((((((hm->getHeader()[0] << 8)
	    | hm->getHeader()[1]) << 8)
	    | hm->getHeader()[2]) << 8)
	    | hm->getHeader()[3]) == 0xbaf01388);
}
