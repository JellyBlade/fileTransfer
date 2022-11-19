#ifndef SEQUENCE_NUMBER_H
#define SEQUENCE_NUMBER_H

/**
 * @brief Struct for managing sequence number and sent/received flags.
 * On receiver side, sent is used for acknowledging, and received is used for
 * delivering the sequence numbers.
 */
struct SequenceNumber {
  int sequence;
  bool received;
  bool sent;
};

#endif // SEQUENCE_NUMBER_H