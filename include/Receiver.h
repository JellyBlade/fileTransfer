#ifndef RECEIVER_H
#define RECEIVER_H

#include <vector>

#include "SlidingWindow.h"
#include "SequenceNumber.h"

/**
 * @brief Struct to store sequence number and out-of-order received sequences for
 * acknowledgement back to the sender.
 */
struct ReceiverAck {
  int sequence;
  std::vector<int> disorderedSeqs;
};

/**
 * @brief The receiver portion of frame header's sequence number management according
 * to selective repeat sliding window protocol.
 */
class Receiver {
 public:
  /**
   * @brief Construct a new Selective Repeat Receiver with a sequence length
   * of b bits.
   * @param b number of bits to use for the sequence number.
   */
  Receiver(int b = 8);

  /**
   * @brief Destroy the Selective Repeat Receiver
   * 
   */
  ~Receiver();

  /**
   * @brief receive a sequence number s, possibly adding it to the sliding window.
   * If it is less than the sliding window's current sequence number, it will be discarded.
   * If it is equal to to the current sequence + 1 and the window can be advanced,
   * it will be received and added to the sliding window.
   * Otherwise, it will be added to disorderedSeqs.
   * 
   * @param s sequence number received.
   */
  void receive(int s);

  /**
   * @brief Returns the next deliverable sequential sequence in the sliding window. 
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
   * @brief returns the next sequence number to be acknowledged.
   * 
   * @return int - sequence number to be acknowledged.
   */
  int getAcknowledgable();

  /**
   * @brief Sends acknowledgement for the highest sequence available to acknowledge.
   * Also sends a vector containing a list of out-of-order sequences received.
   * 
   * @return ReceiverAck - struct containing sequence to ack and disordered sequences.
   */
  ReceiverAck acknowledge();

 private:
  int lastAck = -1;
  std::vector<int> disorderedSeqs;
  SlidingWindow* window;
};

#endif // RECEIVER_H
