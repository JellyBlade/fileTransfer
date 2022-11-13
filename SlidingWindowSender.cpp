#include <cmath>
#include <deque>
#include <vector>
#include <algorithm>

#include "SlidingWindowSender.h"

SlidingWindowSender::SlidingWindowSender(int b = 8) : bitSize{b} {
  maxSeq = pow(2, b) - 1;
};

bool SlidingWindowSender::canAdvance() {
  return seqCount() < windowSize;
}

int SlidingWindowSender::advance() {
  if (canAdvance()) {
    if (currentSeq + 1 >= maxSeq) { currentSeq = -1; }
    windowSequences.push_back((SequenceNumber) {++currentSeq, false});
    return currentSeq;
  }
  return -1;
}

void SlidingWindowSender::setWindowSize(int n) {
  if (n > maxSeq) {
    windowSize = maxSeq;
  } else {
    windowSize = n;
  }
}

void SlidingWindowSender::setBitSize(int b) {
  int newMax = pow(2, b) - 1;
  bitSize = b;
  if (currentSeq > newMax) {
    currentSeq = newMax;
  }
}

bool SlidingWindowSender::acknowledge(int s, std::vector<int> received) {
  // likely a way we can simplify this
  if (s == windowSequences.front().sequence) {
    windowSequences.pop_front();
  } else if (s > windowSequences.front().sequence) {
    for (int i = 0; i < s - windowSequences.front().sequence; i++) {
      windowSequences.pop_front();
    }
  } else { return false; }
  // Remove received frames from the sliding window in order
  while (!windowSequences.empty() && windowSequences.front().received) {
    windowSequences.pop_front();
  }
  // This might not work at all lol
  for (int r : received) {
    auto it = std::find_if(windowSequences.begin(), windowSequences.end(),
      [r] (SequenceNumber s) { return s.sequence == r; });
    if (it != windowSequences.end()) {
      it->received = true;
    }
  }
  return true;
}

int SlidingWindowSender::seqCount() {
  return getWindowSequences().size();
}

std::deque<SequenceNumber> SlidingWindowSender::getWindowSequences() {
  return windowSequences;
}

int SlidingWindowSender::getCurrentSequence() {
  return currentSeq;
}