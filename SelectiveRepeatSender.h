#ifndef SELECTIVE_REPEAT_SENDER_H
#define SELECTIVE_REPEAT_SENDER_H

#include <vector>

#include "SlidingWindow.h"
#include "SequenceNumber.h"
// ReceiverAck struct. Maybe should move it to it's own header?
#include "SelectiveRepeatReceiver.h"


class SelectiveRepeatSender {
 public:
  /**
   * @brief Construct a new Selective Repeat Sender with a sequence length of b bits.
   * 
   * @param b number of bits to use for the sequence number. Default: 8
   */
  SelectiveRepeatSender(int b = 8);

  /**
   * @brief Destroy the Selective Repeat Sender
   * 
   */
  ~SelectiveRepeatSender();

  /**
   * @brief Sends the next available sequence number. Returns -1 if there isn't
   * any unsent sequence numbers in the window.
   * 
   * @return int sequence number sent or -1
   */
  int send();

  /**
   * @brief Sends all unsent sequence numbers available to send.
   * 
   * @return std::vector<int> list of sequence numbers sent.
   */
  std::vector<int> sendAll();

  /**
   * @brief Gets the number of sent sequence numbers that haven't been ack'd.
   * 
   * @return int number of unack'd sent sequence numbers.
   */
  int getSentCount();

  /**
   * @brief Advances the sliding window and prepares a sequence number for sending.
   * 
   * @return true if a sequence number was available to prepare, false if the
   * sliding window was full.
   */
  bool prepareNext();
  
  /**
   * @brief acknowledge ack.sequence has been received, possibly removing 
   * sequence(s) from the sliding window.
   * Functionality for Sender.
   * 
   * @param ack struct containing the ack'd sequence and a list of out-of-order received sequences.
   * @return true if the acknowledgement removed any sequence(s) from the window.
   * @return false otherwise.
   */
  bool acknowledge(ReceiverAck ack);

 private:
  SlidingWindow* window;

  /**
   * @brief Send a sequence number at the specified i index of the sliding window,
   * or next available if seq[i] is already sent.
   * 
   * @param i 
   * @return int sent sequence.
   */
  int send(int i);
};

#endif // SELECTIVE_REPEAT_SENDER_H