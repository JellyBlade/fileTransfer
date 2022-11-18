#ifndef SELECTIVE_REPEAT_RECEIVER_H
#define SELECTIVE_REPEAT_RECEIVER_H

#include <vector>

#include "SlidingWindow.h"
#include "SequenceNumber.h"

struct ReceiverAck {
  int sequence;
  std::vector<int> disorderedSeqs;
};

class SelectiveRepeatReceiver {
 public:
  /**
   * @brief Construct a new Selective Repeat Receiver with a sequence length
   * of b bits.
   * @param b number of bits to use for the sequence number.
   */
  SelectiveRepeatReceiver(int b = 8);

  /**
   * @brief Destroy the Selective Repeat Receiver
   * 
   */
  ~SelectiveRepeatReceiver();

  /**
   * @brief receive a sequence number s, possibly adding it to the sliding window.
   * If it is less than the last ack'd sequence, it will be discarded.
   * If it is equal to seqToAck + 1, it will be
   * If it is greater, it will be added to disorderedSeqs.
   * 
   * @param s sequence number received.
   */
  void receive(int s);

  /**
   * @brief Returns the next deliverable sequence in the sliding window. 
   * Does not deliver/remove it.
   * Returns -1 if there is no deliverable sequence.
   * 
   * @return int - next deliverable sequence in the sliding window.
   */
  int getDeliverableSeq();

  /**
   * @brief Delivers the sequence number, removing it from the window if it has
   * been acknowledged as well.
   * Returns -1 if there is nothing to deliver.
   * 
   * @return int - sequence number delivered, or -1 if nothing to deliver.
   */
  int deliver();

  /**
   * @brief returns the next sequence to be acknowledged.
   * 
   * @return int - sequence number to be acknowledged.
   */
  int getAcknowledgable();

  /**
   * @brief Acknowledges the next in-order sequence that can be acknowledged.
   * Also sends a vector containing a list of out-of-order sequences received.
   * 
   * @return ReceiverAck - struct containing sequence to ack and disordered sequences.
   */
  ReceiverAck acknowledge();

  SlidingWindow* getWindow();

 private:
  std::vector<int> disorderedSeqs;
  SlidingWindow* window;
};

#endif // SELECTIVE_REPEAT_RECEIVER_H
