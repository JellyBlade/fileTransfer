#ifndef SEQUENCE_NUMBER_H
#define SEQUENCE_NUMBER_H

struct SequenceNumber {
  int sequence;
  bool received;
  bool sent;
};

#endif // SEQUENCE_NUMBER_H