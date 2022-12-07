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
  int seqNum = windowSequences->front()->sequence;
  if (s == seqNum) {
    SequenceNumber* delSeq = windowSequences->front();
    windowSequences->pop_front();
    delete delSeq;
  } else if (s > seqNum && windowSequences->size() >= s - seqNum) {
    int delCount = s - seqNum;
    for (int i = 0; i < delCount; i++) {
      SequenceNumber* delSeq = windowSequences->front();
      windowSequences->pop_front();
      delete delSeq;
    }
  } else { return false; }
  // Remove any sent and acknowledged sequence numbers to free up window space.
  for (auto it = windowSequences->begin(); it != windowSequences->end();) {
    if ((*it)->received) {
      SequenceNumber* delSeq = (*it);
      it = windowSequences->erase(it);
      delete delSeq;
    } else { ++it; }
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
