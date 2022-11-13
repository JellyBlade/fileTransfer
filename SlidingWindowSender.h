#ifndef SlidingWindowSender_H
#define SlidingWindowSender_H

#include <deque>
#include <vector>

struct SequenceNumber {
  int sequence;
  bool received;
};

class SlidingWindowSender {
 public:
  /**
   * @brief Construct a new Sliding Window object with a sequence length of b bits.
   * 
   * @param b number of bits to use for the sequence number. Default: 8
   */
  SlidingWindowSender(int b = 8);

  /**
   * @brief checks if there is an empty space in the sliding window.
   * 
   * @return true if there is space in the sliding window to add a frame.
   * @return false otherwise.
   */
  bool canAdvance();

  /**
   * @brief advances the sliding window by one position if possible, and returns
   * the new sequence number.
   * 
   * @return int - new sequence position. Returns -1 if the sliding window cannot
   * advance. 
   */
  int advance();

  /**
   * @brief Set the size of the sliding window. If n is larger than 2^b-1, where
   * b is the number of bits used by the sequence, the window size is set to
   * 2^b-1. 
   * 
   * @param n new sliding window size.
   */
  void setWindowSize(int n);

  /**
   * @brief Set the number of bits the sliding window will use for its sequence.
   * If the current sequence is greater than 2^b-1, the current sequence will
   * set to 2^b-1.
   * 
   * @param b number of bits to use for the sequence number.
   */
  void setBitSize(int b);
  
  /**
   * @brief acknowledge sequence s has been received, possibly removing 
   * sequence(s) from the sliding window.
   * Functionality for Sender.
   * 
   * @param s sequence to acknowledge.
   * @param received list of out-of-order sequences received by the receiver.
   * @return true if the acknowledgement removed any sequence(s) from the window.
   * @return false otherwise.
   */
  bool acknowledge(int s, std::vector<int> received);

  /**
   * @brief Returns the number of unacknowledged sequences in the sliding window.
   * 
   * @see getWindowSequences()
   * @return int - number of unacknowledged sequences in the window.
   */
  int seqCount();

  /**
   * @brief Returns a copy of the vector containing the unacknowledged sequence numbers in 
   * the sliding window.
   * @return std::vector<int> - vector containing unacknowledged sequence numbers.
   */
  std::deque<SequenceNumber> getWindowSequences();

  /**
   * @brief Returns the current highest sequence position.
   * 
   * @return int - current highest sequence position
   */
  int getCurrentSequence();

 private:
  int bitSize;
  int windowSize = 0;
  int currentSeq = 0;
  int maxSeq;
  std::deque<SequenceNumber> windowSequences;
};

#endif // SlidingWindowSender_H