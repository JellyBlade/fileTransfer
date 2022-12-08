#include "HeaderManager.h"
#include <vector>
#include <cstdint>

HeaderManager::HeaderManager() {
  std::fill(header.begin(), header.end(), 0);
}

HeaderManager& HeaderManager::operator=(const HeaderManager &other) {
  header = other.getHeader();
  return *this;
}

std::vector<uint8_t> HeaderManager::getHeader() const {
  return header;
}

int HeaderManager::getType() {
  // shift over 6 bits to get type
  return header[TYPE_START] >> 6;
}

void HeaderManager::setType(int type) {
  // zero out type
  header[TYPE_START] &= 0x3f;
  if (type < 0 || type > 3) {
    return;
  }
  header[TYPE_START] |= (type << 6);
}

int HeaderManager::getTR() {
  // zero out type and shift over 5 to get TR value
  return (header[TYPE_START] & 0x3f) >> 5;
}

bool HeaderManager::isTrunc() {
  return getTR();
}

void HeaderManager::setTR(int tr) {
  // zero out TR
  header[TYPE_START] &= 0xdf;
  if (tr != 0 && tr != 1) {
    return;
  }
  header[TYPE_START] |= (tr << 5);
}

int HeaderManager::getWindow() {
  // zero out type and TR. No shifting required.
  return header[TYPE_START] & 0x1f;
}

void HeaderManager::setWindow(int window) {
  // zero out window
  header[TYPE_START] &= 0xe0;
  if (window > 31 || window < 0) {
   return;
  }
  header[TYPE_START] |= window;
}

int HeaderManager::getSeqNum() {
  // entire byte is seqNum
  return header[SEQ_START];
}

void HeaderManager::setSeqNum(int seqNum) {
  // zero out seqNum
  header[SEQ_START] &= 0x00;
  if (seqNum > 255 || seqNum < 0) {
    return;
  }
  header[SEQ_START] |= seqNum;
}

int HeaderManager::getLength() {
  // combine length's bytes by shifting most sig by 8 and ORing with least sig.
  return (header[LENGTH_START] << 8) | header[LENGTH_END];
}

void HeaderManager::setLength(int length) {
  // zero out length
  header[LENGTH_START] &= 0x00;
  header[LENGTH_END] &= 0x00;
  if (length > 512 || length < 0) {
    return;
  }
  header[LENGTH_START] |= (length >> 8);
  header[LENGTH_END] |= (length & 0x00ff);
}
