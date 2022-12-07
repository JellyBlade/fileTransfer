#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include <vector>
#include <cstdint>

#include "HeaderManager.h"
#include "TimestampManager.h"
#include "CRCManager.h"
#include "PayloadManager.h"

/**
 * @brief Manages the packet structure and data.
 * 
 */
class PacketManager {
 public:
  
  /**
   * @brief Construct a new Packet Manager object
   * 
   */
  PacketManager();


 private:
  const int PTYPE_DATA = 1;
  const int PTYPE_ACK = 2;
  const int PTYPE_NACK = 3;
  HeaderManager header;
  TimestampManager timestamp;
  CRCManager crc1;
  PayloadManager payload;
  CRCManager crc2;
};

#endif // PACKETMANAGER_H