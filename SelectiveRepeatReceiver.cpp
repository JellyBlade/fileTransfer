#include <deque>
#include <vector>

#include "SlidingWindow.h"
#include "SequenceNumber.h"
#include "SelectiveRepeatReceiver.h"

SelectiveRepeatReceiver::SelectiveRepeatReceiver(int b = 8) {
  window = new SlidingWindow(b);
}

SelectiveRepeatReceiver::~SelectiveRepeatReceiver() {
  delete window;
}

void SelectiveRepeatReceiver::receive(int s) {
  if (s <= getAcknowledgable()) { return; }
  if (s == getAcknowledgable() + 1) {
    window->advance(s);
  } else {
    disorderedSeqs.push_back(s);
  }
}

int SelectiveRepeatReceiver::getDeliverableSeq() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    if (!seqs->at(i)->received) {
      return seqs->at(i)->sequence;
    }
  }
  return -1;
}

int SelectiveRepeatReceiver::deliver() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    // if not delivered
    if (!seq->received) {
      int deliverSeq = seq->sequence;
      // if acknowledged, delete. This might come back to haunt us.
      if (seq->sent) {
        seqs->erase(seqs->begin() + i);
        delete seq;
      }
      return deliverSeq;
    }
  }
  return -1;
}

int SelectiveRepeatReceiver::getAcknowledgable() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  int toAck = -1;
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    if (!seq->sent) {
      toAck = seq->sequence;
    }
  }
  return toAck;
}

ReceiverAck SelectiveRepeatReceiver::acknowledge() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  int toAck = -1;
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    if (!seq->sent) {
      seq->sent = true;
      toAck = seq->sequence;
    }
  }
  return (ReceiverAck) {toAck, disorderedSeqs};
}