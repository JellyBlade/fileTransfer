#ifndef SEQUENCE_NUMBER_H
#define SEQUENCE_NUMBER_H

/**
 * @brief Struct for managing sequence number and sent/received flags.
 * On receiver side, sent is used for acknowledging, and received is used for
 * delivering the sequence numbers.
 */
struct SequenceNumber {
  SequenceNumber(int seq, bool rec, bool snt) {
    sequence = seq;
    received = rec;
    sent = snt;
  }
  int sequence = 0;
  bool received = false;
  bool sent = false;
};

#endif // SEQUENCE_NUMBER_H
