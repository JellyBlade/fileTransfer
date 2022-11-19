#include <cmath>
#include <deque>

#include "SequenceNumber.h"
#include "SlidingWindow.h"

SlidingWindow::SlidingWindow(int b) : bitSize{b} {
  windowSequences = new std::deque<SequenceNumber*>;
  maxSeq = pow(2, b) - 1;
  windowSize = b;
}

SlidingWindow::~SlidingWindow() {
  while(!windowSequences->empty()) {
    SequenceNumber* delSeq = windowSequences->back();
    windowSequences->pop_back();
    delete delSeq;
  }
  delete windowSequences;
}

int SlidingWindow::seqCount() {
  return getWindowSequences()->size();
}

bool SlidingWindow::canAdvance() {
  return seqCount() < windowSize;
}

int SlidingWindow::advance() {
  if (canAdvance()) {
    currentSeq = (currentSeq + 1) % (maxSeq + 1);
    windowSequences->push_back(new (SequenceNumber) {currentSeq, false, false});
    return currentSeq;
  }
  return -1;
}

void SlidingWindow::setWindowSize(int n) {
  if (n > maxSeq) {
    windowSize = maxSeq;
  } else {
    windowSize = n;
  }
}

int SlidingWindow::getWindowSize() {
  return windowSize;
}

void SlidingWindow::setBitSize(int b) {
  maxSeq = pow(2, b) - 1;
  bitSize = b;
}

int SlidingWindow::getBitSize() {
  return bitSize;
}

std::deque<SequenceNumber*>* SlidingWindow::getWindowSequences() {
  return windowSequences;
}

int SlidingWindow::getCurrentSequence() {
  return currentSeq;
}
