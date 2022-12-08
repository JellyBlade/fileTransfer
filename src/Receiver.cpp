#include <deque>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "SlidingWindow.h"
#include "SequenceNumber.h"
#include "Receiver.h"

Receiver::Receiver(int b) {
  window = new SlidingWindow(b);
}

Receiver::~Receiver() {
  delete window;
}

void Receiver::receive(int s) {
  if (s <= window->getCurrentSequence()) { return; }
  if (s == window->getCurrentSequence() + 1 && window->canAdvance()) {
    int ws = window->advance();
    if (s != ws) {
      throw std::runtime_error("Expected sequence " + std::to_string(s) + ", got " + std::to_string(ws));
    }
    std::sort(disorderedSeqs.begin(), disorderedSeqs.end());
    while (disorderedSeqs.size() > 0 && disorderedSeqs.at(0) == ++ws) {
      disorderedSeqs.erase(disorderedSeqs.begin());
      window->advance();
    }
  } else {
    if (std::find(disorderedSeqs.begin(), disorderedSeqs.end(), s) == disorderedSeqs.end()) {
      disorderedSeqs.push_back(s);
    }
  }
}

int Receiver::getDeliverableSeq() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    if (!seqs->at(i)->received) {
      return seqs->at(i)->sequence;
    }
  }
  return -1;
}

int Receiver::deliver() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    // if not delivered
    if (!seq->received) {
      int deliverSeq = seq->sequence;
      seq->received = true;
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

int Receiver::getAcknowledgable() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    if (seqs->size() > i + 1 && seqs->at(i+1)->sequence == seq->sequence + 1) {
      continue;
    } else if (!seq->sent) {
      return seq->sequence;
    }
  }
  return lastAck;
}


ReceiverAck Receiver::acknowledge() {
  std::deque<SequenceNumber*>* seqs = window->getWindowSequences();
  for (int i = 0; i < seqs->size(); i++) {
    SequenceNumber* seq = seqs->at(i);
    if (!seq->sent) {
      seq->sent = true;
      if (seqs->size() > i + 1 && seqs->at(i+1)->sequence == seq->sequence + 1) {
        if (seq->received) {
          seqs->erase(seqs->begin() + i);
          delete seq;
          i--;
        }
        continue;
      }
      ReceiverAck ack = (ReceiverAck) {seq->sequence, disorderedSeqs};
      if (seq->received) {
        seqs->erase(seqs->begin() + i);
        delete seq;
      }
      lastAck = ack.sequence;
      return ack;
    }
  }
  return (ReceiverAck) {lastAck, disorderedSeqs};
}
