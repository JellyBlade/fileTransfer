#ifndef TIMESTAMPMANAGER_H
#define TIMESTAMPMANAGER_H

#include <vector>
#include <cstdint>

class TimestampManager {
 public:
  
  /**
   * @brief Construct a new Timestamp Manager object. Sets the timestamp
   * to the current system time.
   * This implementation of the timestamp field uses network byte-order (big-endian)
   */
  TimestampManager();

  /**
   * @brief Sets the timestamp to the given UNIX time integer.
   * 
   * @param unix UNIX time integer to store
   */
  void set(int unix);

  /**
   * @brief Sets the timestamp as the current time of the system as a UNIX time
   * integer.
   */
  void set();

  /**
   * @brief Returns the timestamp vector.
   * 
   * @return std::vector<uint8_t> timestamp vector
   */
  std::vector<uint8_t> getBytes();

  /**
   * @brief Returns the timestamp as a UNIX time integer.
   * 
   * @return int UNIX time
   */
  int getUNIX();
 private:
  std::vector<uint8_t> timestamp;
};

#endif // TIMESTAMPMANAGER_H