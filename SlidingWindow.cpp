#include <cmath>
#include <deque>

#include "SequenceNumber.h"
#include "SlidingWindow.h"

SlidingWindow::SlidingWindow(int b) : bitSize{b} {
  maxSeq = pow(2, b) - 1;
}

SlidingWindow::~SlidingWindow() {
  int delCount = windowSequences->size();
  for (int i = 0; i < delCount; i++) {
    SequenceNumber* delSeq = windowSequences->front();
    windowSequences->pop_front();
    delete delSeq;
  }
}

int SlidingWindow::seqCount() {
  return getWindowSequences()->size();
}

bool SlidingWindow::canAdvance() {
  return seqCount() < windowSize;
}

int SlidingWindow::advance() {
  if (canAdvance()) {
    // mod maxSeq + 1 to allow for currentSeq = maxSeq
    windowSequences->push_back(new (SequenceNumber) {++currentSeq % (maxSeq + 1), false, false});
    return currentSeq;
  }
  return -1;
}

int SlidingWindow::advance(int s) {
  if (canAdvance()) {
    windowSequences->push_back(new (SequenceNumber) {s % (maxSeq + 1), false, false});
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

void SlidingWindow::setBitSize(int b) {
  int newMax = pow(2, b) - 1;
  bitSize = b;
  if (currentSeq > newMax) {
    currentSeq = newMax;
  }
}

std::deque<SequenceNumber*>* SlidingWindow::getWindowSequences() {
  return windowSequences;
}

int SlidingWindow::getCurrentSequence() {
  return currentSeq;
}
