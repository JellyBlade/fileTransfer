#ifndef CRCMANAGER_H
#define CRCMANAGER_H

#include <vector>
#include <cstdint>

#include "HeaderManager.h"
#include "PayloadManager.h"

/**
 * @brief Manages the 4-byte CRC32 checksum for the header and payload.
 */
class CRCManager {
 public:
  
  /**
   * @brief Construct a new CRCManager, generating an initial checksum.
   * 
   */
  CRCManager();

  /**
   * @brief Construct a new CRCManager, generating a checksum for the header
   * contained in the given HeaderManager.
   * 
   * @param h HeaderManager containing the header to turn into a checksum.
   */
  CRCManager(HeaderManager h);

  /**
   * @brief Construct a new CRCManager, generating a checksum for the header
   * contained in the given uint8_t vector.
   * 
   * @param h Vector containing the header to turn into a checksum.
   */
  CRCManager(std::vector<uint8_t> h);

  /**
   * @brief Construct a new CRCManager, generating a checksum for the payload
   * contained in the given PayloadManager.
   * 
   * @param p 
   */
  CRCManager(PayloadManager p);

  /**
   * @brief Generates a CRC32 checksum for the given vector of uint8_t's.
   * 
   */
  void generate(std::vector<uint8_t> h);

  /**
   * @brief Returns the CRC32 checksum
   * 
   * @return unsigned long CRC32 checksum
   */
  unsigned long getCRC() const;

   /**
   * @brief Returns the CRC32 checksum as a vector of uint8_t's
   * 
   * @return unsigned long CRC32 checksum as 4 bytes.
   */
  std::vector<uint8_t> getCRCBytes();

  /**
   * @brief Compare the CRC in the given CRCManager to this one.
   * 
   * @param c CRCManager class containing the CRC32 checksum to compare
   * @return true if the checksums are equal
   * @return false otherwise
   */
  bool compare(CRCManager& c);

  /**
   * @brief Compare the given CRC32 checksum to this one.
   * 
   * @param c CRC32 checksum to compare
   * @return true if the checksums are equal
   * @return false otherwise
   */
  bool compare(unsigned long c);

  /**
   * @brief operator overload to compare CRC32 checksums between CRCManagers.
   * 
   * @param c CRCManager to compare checksums against.
   * @return true if the checksums are equal
   * @return false otherwise
   */
  bool operator==(const CRCManager& c) const;
 private:
  unsigned long crc;
};


#endif // CRCMANAGER_H