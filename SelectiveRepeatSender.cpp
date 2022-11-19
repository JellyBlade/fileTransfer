#include <deque>
#include <vector>
#include <algorithm>

#include "SequenceNumber.h"
#include "SelectiveRepeatSender.h"
#include "SelectiveRepeatReceiver.h"

SelectiveRepeatSender::SelectiveRepeatSender(int b) {
  window = new SlidingWindow(b);
};

SelectiveRepeatSender::~SelectiveRepeatSender() {
  delete window;
}

int SelectiveRepeatSender::send() {
  return send(0);
}

int SelectiveRepeatSender::send(int i) {
  auto seqs = window->getWindowSequences();
  if (seqs->size() < 1) { return -1; }
  SequenceNumber* seq = seqs->at(i);
  while (seq->sent) {
    if (++i >= seqs->size()) {
      return -1;
    }
    seq = seqs->at(i);
  }
  seq->sent = true;
  return seq->sequence;
}

std::vector<int> SelectiveRepeatSender::sendAll() {
  std::vector<int> sent;
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    if (!seqs->at(i)->sent) {
      sent.push_back(send(i));
    }
  }
  return sent;
}

int SelectiveRepeatSender::getSentCount() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  int count = 0;
  for (int i = 0; i < seqs->size(); i++) {
    if (seqs->at(i)->sent) {
      count++;
    }
  }
  return count;
}

bool SelectiveRepeatSender::prepareNext() {
  if (window->advance() != -1) {
    return true;
  } else { return false; }
}

bool SelectiveRepeatSender::acknowledge(ReceiverAck ack) {
  // remove 1 or more sequences from the sliding window when acknowledged
  // if s is 4 and front of deque is 2, it'll remove 2, 3, and 4. Assumes the
  // deque is in order (should be).
  int s = ack.sequence;
  std::vector<int> disorderedSeqs = ack.disorderedSeqs;
  std::deque<SequenceNumber*>* windowSequences = window->getWindowSequences();
  if (windowSequences->empty()) { return false; }
  SequenceNumber* seq = windowSequences->front();
  if (s == seq->sequence) {
    windowSequences->pop_front();
    delete seq;
  } else if (s > seq->sequence) {
    int delCount = s - seq->sequence;
    for (int i = 0; i < delCount; i++) {
      SequenceNumber* delSeq = windowSequences->front();
      windowSequences->pop_front();
      delete delSeq;
    }
  } else { return false; }
  // Remove any sent and acknowledged sequence numbers to free up window space.
  for (int i = 0; i < windowSequences->size(); i++) {
    if (windowSequences->at(i)->received) {
      SequenceNumber* delSeq = windowSequences->at(i);
      windowSequences->erase(windowSequences->begin() + i);
      delete delSeq;
      i--;
    }
  }
  // Set sequence numbers in the window to transmitted to show they've been
  // properly received by the receiver.
  for (int r : disorderedSeqs) {
    auto it = std::find_if(windowSequences->begin(), windowSequences->end(),
      [r] (SequenceNumber* s) { return s->sequence == r; });
    if (it != windowSequences->end()) {
      (*it)->received = true;
    }
  }
  return true;
}


sender -> 1234567
receiver <- 1234 67 -> ack(4, [6, 7]);
sender <- ack(4, [6,7])
sender -> 589 10 11