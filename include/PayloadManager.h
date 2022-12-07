#ifndef PAYLOADMANAGER_H
#define PAYLOADMANAGER_H

#include <vector>
#include <cstdint>

/**
 * @brief Manages the (up to) 512-byte packet payload
 * 
 */
class PayloadManager {
 public:

  /**
   * @brief Construct a new Payload Manager with a payload size of zero.
   * 
   */
  PayloadManager();

  /**
   * @brief Construct a new Payload Manager with a payload size of s bytes;
   * 
   */
  PayloadManager(int s);

  /**
   * @brief Adds the given uint8_t to the payload
   * 
   * @param p data to add to the payload
   * @return true if the data was successfully added to the payload
   * @return false otherwise
   */
  bool add(uint8_t p);

  /**
   * @brief Returns the payload vector.
   * 
   * @return std::vector<uint8_t> payload
   */
  std::vector<uint8_t> get();

 private:
  std::vector<uint8_t> payload;
  int size = 0;
};

#endif // PAYLOADMANAGER_H