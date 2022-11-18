#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <deque>

#include "SequenceNumber.h"

class SlidingWindow {
 public:
  /**
   * @brief Construct a new Sliding Window with a sequence length of b bits.
   * 
   * @param b number of bits to use for the sequence number. Default: 8
   */
  SlidingWindow(int b = 8);

  /**
   * @brief Destroy the Sliding Window and all SequenceNumbers in it.
   * 
   */
  ~SlidingWindow();

  /**
   * @brief checks if there is an empty space in the sliding window.
   * 
   * @return true if there is space in the sliding window to add a sequence number.
   * @return false otherwise.
   */
  bool canAdvance();

  /**
   * @brief advances the sliding window by one position if possible, and returns
   * the new sequence number.
   * Returns -1 if the sliding window cannot advance.
   * 
   * @return int - new sequence position. Returns -1 if the sliding window cannot
   * advance. 
   */
  int advance();

  /**
   * @brief advances the sliding window by one position if possible, setting the
   * new sequence number to s.
   * If s is larger than the max allowed sequence number, it will be equal to
   * s % (maximum sequence number size)
   * Returns -1 if the sliding window cannot advance.
   * 
   * @param s sequence number to use.
   * @return int - new sequence number. Returns -1 if the sliding window cannot
   * advance.
   */
  int advance(int s);

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
   * @brief Returns the number of unacknowledged sequences in the sliding window.
   * 
   * @see SlidingWindow::getWindowSequences()
   * @return int - number of unacknowledged sequences in the window.
   */
  int seqCount();

    /**
   * @brief Returns a pointer to the deque containing the unacknowledged sequence numbers in 
   * the sliding window.
   * @return std::deque<SequenceNumber>* - deque containing unacknowledged sequence numbers.
   */
  std::deque<SequenceNumber*>* getWindowSequences();

  /**
   * @brief Returns the current highest sequence position.
   * 
   * @return int - current highest sequence position
   */
  int getCurrentSequence();

 private:
  int bitSize;
  int windowSize = 0;
  int maxSeq = 0;
  int currentSeq = 0;
  std::deque<SequenceNumber*>* windowSequences;
};

#endif // SLIDING_WINDOW_H
